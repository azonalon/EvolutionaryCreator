#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>
#include "shader/ShaderProgram.hpp"
#include "physics/ElasticModel.hpp"

class CoordinateGridRender {
 public:
  static constexpr std::array<double, 8> coordinateData = {-1, -1, -1, 1,
                                                           1,  1,  1, -1};
  double scale = 1;
  double spacing = 1;
  GLuint buffer, vao;
  ShaderProgram cgs;
  CoordinateGridRender(ShaderContext &context, double spacing)
      : cgs("resources/shaders/Grid.vert", "", "resources/shaders/Grid.frag",
            context) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &buffer);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(double), &coordinateData[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glBindVertexArray(0);
  };
  void draw() {
    cgs.enable();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
  };
};

class ElasticModelRender {
 public:
  GLuint texture;
  ElasticModel *m;
  ShaderProgram ems;
  ShaderProgram bbs;
  ShaderProgram fws;
  const unsigned restartIndex = 65535;
  bool drawBoundingBoxes = true;
  bool drawForceVectors = true;
  GLuint vertexBuffer, indexBuffer, uvBuffer, vao, vaoBoundingBox, sBuffer, forceBuffer, vaoForces;
  ElasticModelRender(ElasticModel *m, ShaderContext &context,
                     const unsigned char *texPixels, unsigned texWidth,
                     unsigned texHeight, const double* texCoords)
      : m(m),
        ems("resources/shaders/ElasticMesh.vert",
            "resources/shaders/ElasticMesh.geom",
            "resources/shaders/ElasticMesh.frag", context),
        bbs("resources/shaders/BoundingBox.vert",
            "resources/shaders/BoundingBox.geom",
            "resources/shaders/BoundingBox.frag", context),
        fws("resources/shaders/Flow.vert",
            "",
            "resources/shaders/Flow.frag", context) 
            {
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &indexBuffer);
    glGenBuffers(1, &uvBuffer);
    glGenBuffers(1, &sBuffer);
    glGenBuffers(1, &forceBuffer);
    glGenVertexArrays(1, &vao);
    glGenVertexArrays(1, &vaoForces);
    glGenVertexArrays(1, &vaoBoundingBox);

    // LOAD TEXTURE INTO PIPELINE
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
                 // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    assert(texPixels != NULL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, texPixels);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m->vertexCount() * sizeof(double) * 2,
                 m->x2.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, m->vertexCount() * sizeof(double) * 2,
                 texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, sBuffer);
    glBufferData(GL_ARRAY_BUFFER, m->vertexCount() * sizeof(double) * 1,
                 texCoords, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 m->triangleCount() * sizeof(unsigned int) * 3, m->Te.data(),
                 GL_STATIC_DRAW);

    glBindVertexArray(vaoForces);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, forceBuffer);
    glBufferData(GL_ARRAY_BUFFER, m->vertexCount() * sizeof(double) * 2,
                 m->g.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 0, 0);

    glBindVertexArray(0);
  }
  void draw() {
    ems.enable();
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m->vertexCount() * sizeof(double) * 2,
                    m->x2.data());
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    // vertex coordinates
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    // uv coordinates for textures
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    // stretch factors
    std::vector<double> sVertexValues(m->vertexCount(), 0.0);
    for(unsigned i=0; i<m->Te.rows(); i++) {
      // auto& triangle = m->Te[i];
      sVertexValues[m->Te(i ,0)] += m->S[i]/3.0;
      sVertexValues[m->Te(i ,1)] += m->S[i]/3.0;
      sVertexValues[m->Te(i ,2)] += m->S[i]/3.0;
    }
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, sBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m->vertexCount() * sizeof(double),
                    sVertexValues.data());
    glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 0, 0);
    glDrawElements(GL_TRIANGLES, 3 * m->triangleCount(), GL_UNSIGNED_INT,
                   nullptr);
    // TODO: enable

    if (drawForceVectors) {
      fws.enable();
      glBindVertexArray(vaoForces);
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
      glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, forceBuffer);
      glBufferSubData(GL_ARRAY_BUFFER, 0, m->vertexCount() * sizeof(double) * 2,
                      m->modelForces.data());
      glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 0, 0);
      glDrawArrays(GL_POINTS, 0, m->vertexCount());
    }

    if (drawBoundingBoxes) {
      glBindVertexArray(vaoBoundingBox);
      bbs.enable();
      std::vector<double> coords;
      for (auto &co : m->collisionObjects) {
        auto bb = co->boundingBoxUntransformed();
        Eigen::Vector2d a(bb.x0, bb.y0),b(bb.x0,bb.y1),c(bb.x1,bb.y0),d(bb.x1, bb.y1);
        a = co->aux.inverse()*a; b = co->aux.inverse()*b; c = co->aux.inverse()*c; d = co->aux.inverse()*d;
        coords.insert(coords.end(),
                      {a.x(), a.y(), b.x(), b.y(), c.x(), c.y(), b.x(), b.y(), c.x(), c.y(), d.x(), d.y()});
      }

      GLuint vb, tb;
      glGenBuffers(1, &vb);
      glGenBuffers(1, &tb);
      // glGenBuffers(1, &ib);
      glBindBuffer(GL_ARRAY_BUFFER, vb);
      glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(double), &coords[0],
                   GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
      glDrawArrays(GL_TRIANGLES, 0, coords.size()/2);
      glDeleteBuffers(1, &vb);
    }
    glBindVertexArray(0);
  }
};