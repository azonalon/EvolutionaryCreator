#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include <functional>
#include <glm/gtc/matrix_transform.hpp>

class View {

    double zoom=1, aspect=1;
    double x0=0, y0=0, x1=0, y1=0, xC=0, yC=0;
    bool dragging = false;
    glm::mat4 viewMatrix;
    GLFWwindow* window;
public:

    View(GLFWwindow* w): window(w) {
        installCallbacks(window);
    }

    static void mouseButtonCallback(View* view, GLFWwindow* w, int button, int action, int /*mods*/)
    {
        if(ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
            glfwGetCursorPos(w, &view->xC, &view->yC);
            view->x1 = view->x0;
            view->y1 = view->y0;
            view->dragging = true;
        }
        else if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT) {
            view->dragging = false;
        }
    }
    static void cursorPosCallback(View* view, GLFWwindow* window, double x, double y)
    {
        if(ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        if(view->dragging) {
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
        glViewport(0, 0, w, h);
    }


    void installCallbacks(GLFWwindow* window)
    {
        glfwSetWindowUserPointer(window, this);

        #define genericCallback(functionName)\
        [](GLFWwindow* window, auto... args) {\
            auto pointer = static_cast<View*>(glfwGetWindowUserPointer(window));\
            if (pointer->functionName) View::functionName(pointer, window, args...);\
        }

        glfwSetMouseButtonCallback(window, genericCallback(mouseButtonCallback));
        glfwSetScrollCallback(window, genericCallback(scrollCallback));
        glfwSetKeyCallback   (window, genericCallback(keyCallback));
        glfwSetCharCallback  (window, genericCallback(charCallback));
        glfwSetCursorPosCallback (window, genericCallback(cursorPosCallback));
        glfwSetWindowSizeCallback(window, genericCallback(windowSizeCallback));
    }
};
