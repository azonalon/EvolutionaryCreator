#include "render/Render.hpp"
#include <array>

class StaticObjectRender {
public:
  GLuint texture;
  ShaderProgram wireframeShader;
  unsigned nVertices;
  GLuint vertexBuffer, indexBuffer, uvBuffer, vao;
  GLenum drawType = GL_TRIANGLE_FAN;
  StaticObjectRender(ShaderContext &context, const double *data,
                     unsigned nVertices)
      : wireframeShader("resources/shaders/Wireframe.vert", "",
                        "resources/shaders/Wireframe.frag", context),
        nVertices(nVertices) {
    glGenBuffers(1, &vertexBuffer);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * nVertices * sizeof(double), data,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glBindVertexArray(0);
  }

  void draw() {
    wireframeShader.enable();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices);
    glBindVertexArray(0);
  }
};