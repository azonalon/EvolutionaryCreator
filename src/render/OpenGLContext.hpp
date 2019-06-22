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
};