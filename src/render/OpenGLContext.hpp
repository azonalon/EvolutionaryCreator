#pragma once
#include "ui/ImGuiGlfw.hpp"
#include "ui/ImGuiOpengl.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "view/View.hpp"

class OpenGLContext {
public:
    ImVec4 clear_color = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
    View view;
    GLFWwindow *window;
    void init();
    bool nextFrame();
    void destroy();
    OpenGLContext() {
    };

    glm::vec2 cursorPosWorld() {
        auto pos = ImGui::GetMousePos();
        double w = view.w;
        double h = view.h;
        glm::vec4 v((-1+2*pos.x/w), (1-2*pos.y/h), 0, 1);
        v = glm::inverse(view.shaderContext->viewMatrix)*v;
        return glm::vec2(v.x, v.y);
    }
};