#pragma once
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include <functional>
#include <glm/gtc/matrix_transform.hpp>
#include "shader/ShaderProgram.hpp"

static constexpr bool DEBUG_UI = false;

class View {
public:
    double zoom=1, aspect=1;
    double x0=0, y0=0, x1=0, y1=0, xC=0, yC=0;
    int w, h;// window size
    bool dragging = false;
    ShaderContext* shaderContext=NULL;
    GLFWwindow* window;
    void setWindow(GLFWwindow* w) {
        window = w;
        installCallbacks(window);
    }

    View(ShaderContext* shaderContext): shaderContext(shaderContext) {};
    View(){};

    static void mouseButtonCallback(View* view, GLFWwindow* w, int button, int action, int mods)
    {
        if(ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        if(DEBUG_UI) printf("Mousebutton callback called with action=%d, button=%d, mods=%d\n", action, button, mods);
        if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
            glfwGetCursorPos(w, &view->xC, &view->yC);
            if(DEBUG_UI) printf("Clicked left mouse at %g, %g... \n", view->xC, view->yC);
            view->x1 = view->x0;
            view->y1 = view->y0;
            view->dragging = true;
        }
        else if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT) {
            view->dragging = false;
            if(DEBUG_UI) printf("Stop Dragging... \n");
        }
    }
    static void cursorPosCallback(View* view, GLFWwindow* window, double x, double y)
    {
        if(ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        if(view->dragging) {
            if(DEBUG_UI) printf("Dragging... \n");
            int w, h;
            glfwGetWindowSize(window, &w, &h);
            view->x0 = view->x1 - (x - view->xC)*view->zoom*2/(double)w;
            view->y0 = view->y1 + (y - view->yC)*view->zoom*2/(double)h;
        }
    }

    glm::mat4 computeViewMatrix() {
        return glm::ortho((x0-zoom)*aspect, (x0+zoom)*aspect, (y0-zoom), (y0+zoom));
    }

    static void scrollCallback(View* view, GLFWwindow* w, double xoffset, double yoffset)
    {
        view->zoom  = std::max(0.5, view->zoom+yoffset);
        if(view->zoom > 100) {
            view->zoom = 100;
        }
    }

    static void keyCallback(View* view, GLFWwindow* w, int key, int, int action, int mods)
    {
    }

    static void charCallback(View* view, GLFWwindow* w, unsigned int c)
    {
    }

    static void windowSizeCallback(View* view, GLFWwindow* win, int w, int h)
    {
        view->aspect = (double)w/(double)h;//w/h;
        view->h = h;
        view->w = w;
        glViewport(0, 0, w, h);
    }


    void installCallbacks(GLFWwindow* window)
    {
        if(DEBUG_UI) printf("Installing UI callbacks for glfw...\n");
        glfwSetWindowUserPointer(window, this);

        #define genericCallback(functionName)\
        [](GLFWwindow* window, auto... args) {\
            auto pointer = static_cast<View*>(glfwGetWindowUserPointer(window));\
            if (pointer->functionName) View::functionName(pointer, window, args...);\
        }

        // glfwSetMouseButtonCallback(window, genericCallback(mouseButtonCallback));
        // glfwSetScrollCallback(window, genericCallback(scrollCallback));
        // glfwSetKeyCallback   (window, genericCallback(keyCallback)   );
        // glfwSetCharCallback  (window, genericCallback(charCallback)  );
        // glfwSetCursorPosCallback (window, genericCallback(cursorPosCallback));
        // glfwSetWindowSizeCallback(window, genericCallback(windowSizeCallback));
        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
            View* pointer = static_cast<View*>(glfwGetWindowUserPointer(window));
            View::mouseButtonCallback(pointer, window, button, action, mods);
        });
        glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
            View* pointer = static_cast<View*>(glfwGetWindowUserPointer(window));
            View::scrollCallback(pointer, window, xoffset, yoffset);
        });
        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int arg, int action, int mods) {
            View* pointer = static_cast<View*>(glfwGetWindowUserPointer(window));
            View::keyCallback(pointer, window, key, arg, action, mods);
        });
        glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int c) {
            View* pointer = static_cast<View*>(glfwGetWindowUserPointer(window));
            View::charCallback(pointer, window, c);
        });
        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
            View* pointer = static_cast<View*>(glfwGetWindowUserPointer(window));
            View::cursorPosCallback(pointer, window, x, y);
        });
        glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int w, int h) {
            View* pointer = static_cast<View*>(glfwGetWindowUserPointer(window));
            View::windowSizeCallback(pointer, window, w, h);
        });
    }
};
