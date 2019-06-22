#include "render/Render.hpp"
#include <array>

struct TextureBounds {
  double x0, y0, x1, y1;
};

class ColorMapJet {
 public:
  typedef struct {
    double r, g, b;
  } COLOUR;

  static COLOUR GetColour(double v, double vmin, double vmax) {
    COLOUR c = {1.0, 1.0, 1.0};  // white
    double dv;

    if (v < vmin) v = vmin;
    if (v > vmax) v = vmax;
    dv = vmax - vmin;

    if (v < (vmin + 0.25 * dv)) {
      c.r = 0;
      c.g = 4 * (v - vmin) / dv;
    } else if (v < (vmin + 0.5 * dv)) {
      c.r = 0;
      c.b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
    } else if (v < (vmin + 0.75 * dv)) {
      c.r = 4 * (v - vmin - 0.5 * dv) / dv;
      c.b = 0;
    } else {
      c.g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
      c.b = 0;
    }
    return (c);
  }
};

class FlowRender {
 public:
  GLuint texture;
  ShaderProgram flowShader;
  ShaderProgram vortexShader;
  ShaderProgram textureShader;
  TextureBounds bb;
  GLuint vertexBuffer, uvBuffer, vao, vaoBoundingBox, vb, vaoVortex, vpBuffer, vvBuffer;
  FlowRender(ShaderContext &context, TextureBounds bb, unsigned nX, unsigned nY) : 
    flowShader("resources/shaders/Flow.vert",
              "",
              "resources/shaders/Flow.frag", context) ,
        textureShader("resources/shaders/Texture.vert",
            "",
            "resources/shaders/Texture.frag", context),
        vortexShader("resources/shaders/Vortex.vert",
            "",
            "resources/shaders/Vortex.frag", context),
            bb(bb)
            {
    // glGenBuffers(1, &vertexBuffer);
    glGenVertexArrays(1, &vao);
    glGenVertexArrays(1, &vaoVortex);
    glGenVertexArrays(1, &vaoBoundingBox);

    glGenTextures(1, &texture);
    // LOAD TEXTURE INTO PIPELINE
    glBindTexture(GL_TEXTURE_2D, texture);
    // TODO: Fix color byte format
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nX, nY, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, NULL);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, nX, nY);
                 // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &uvBuffer);
    glGenBuffers(1, &vb);
    glGenBuffers(1, &vpBuffer);
    glGenBuffers(1, &vvBuffer);

    // init bounding box VAO
    Eigen::Vector2d a(bb.x0, bb.y0), b(bb.x0, bb.y1), c(bb.x1, bb.y0),
        d(bb.x1, bb.y1);
    std::vector<double> coords {a.x(), a.y(), b.x(), b.y(), d.x(), d.y(),
                                c.x(), c.y()};

    glBindVertexArray(vaoBoundingBox);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(double), &coords[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glBindVertexArray(0);
  }

  void draw(GLubyte *density, const Eigen::ArrayXXd &x,
            const Eigen::ArrayXXd &y, const Eigen::ArrayXXd &u,
            const Eigen::ArrayXXd &v, unsigned nX, unsigned nY) {
    textureShader.enable();
    glBindTexture(GL_TEXTURE_2D, texture);
    // TODO: Fix color byte format
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nX, nY, GL_RGBA,
                 GL_UNSIGNED_BYTE, density);

    glBindVertexArray(vaoBoundingBox);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    // draw the flow
    flowShader.enable();
    glBindVertexArray(vao);
    std::vector<double> positions(nX*nY*2);
    std::vector<double> uvs(nX*nY*2);
    for(unsigned i=0; i<nX; i++) {
      for(unsigned j=0; j<nY; j++) {
        positions[nX*i*2 + 2*j + 0] = x(i,j);
        positions[nX*i*2 + 2*j + 1] = y(i,j);
        uvs[nX*i*2 + 2*j + 0] = u(i,j);
        uvs[nX*i*2 + 2*j + 1] = v(i,j);
      }
    }

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, nX * nY * sizeof(double) * 2,
                 positions.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, nX * nY * sizeof(double) * 2,
                 uvs.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glDrawArrays(GL_POINTS, 0, nX*nY);
    glBindVertexArray(0);
  }
  void drawVortices(const std::vector<double> &positions,
                    const std::vector<double> &velocities) {
    // Draw the vortices
    vortexShader.enable();
    glBindVertexArray(vaoVortex);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vpBuffer);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(double),
                 positions.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, velocities.size() * sizeof(double),
                 velocities.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 0, 0);
    glDrawArrays(GL_POINTS, 0, positions.size()/2);
    glBindVertexArray(0);
  }
};