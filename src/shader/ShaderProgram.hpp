#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "util/StringFormat.hpp"
#include <glm/glm.hpp>

class ShaderContext {
public:
    GLuint uniformBuffer;
    void setViewMatrix(glm::mat4 m) {
        // glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, &m[0][0]);
        glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &m[0][0]);
    }
    ShaderContext() {
        glGenBuffers(1, &uniformBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, uniformBuffer);

    }
};

class ShaderProgram {
public:
    ShaderContext& context;
    GLuint program, vertex, fragment, geometry;
    static GLuint loadShader(std::string filePath, GLenum shaderType) {
        GLuint shader = glCreateShader(shaderType);

        std::ifstream sourceStream;
        sourceStream.open(filePath);
        std::string source((std::istreambuf_iterator<char>(sourceStream)),
        std::istreambuf_iterator<char>());
        // Get strings for glShaderSource.
        const char* psource[] = {source.c_str()};
        glShaderSource(shader, 1, psource, NULL);
        glCompileShader(shader);
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

            // Provide the infolog in whatever manor you deem best.
            // Exit with failure.
            glDeleteShader(shader); // Don't leak the shader.
            fprintf(stderr, "Could not compile shader: %s", filePath.c_str());
            throw std::runtime_error("");
            return 0;
        }
        return shader;

    }
    ShaderProgram(std::string vertexShader, std::string geometryShader,
                  std::string fragmentShader, ShaderContext& context): context(context) {

        // Read shaders
        if(geometryShader.size() > 1) {
            geometry = loadShader(geometryShader, GL_GEOMETRY_SHADER);
        }
        vertex = loadShader(vertexShader, GL_VERTEX_SHADER);
        fragment = loadShader(fragmentShader, GL_FRAGMENT_SHADER);

        std::cout << "Linking program..." << std::endl;
        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        if(geometryShader.size() > 1) {
            glAttachShader(program, geometry);
        }
        glLinkProgram(program);
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);
            // Don't leak shaders either.
            glDeleteShader(vertex);
            glDeleteShader(fragment);

            // Use the infoLog as you see fit.

            // In this simple program, we'll just leave
            return;
        }

        // Always detach shaders after a successful link.
        glDetachShader(program, vertex);
        glDetachShader(program, fragment);
    }
    void enable() {
        glUseProgram(program);
    }
};
