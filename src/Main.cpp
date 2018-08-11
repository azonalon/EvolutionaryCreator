#include "imgui/imgui.h"
#include "ui/ImGuiGlfw.hpp"
#include "ui/ImGuiOpengl.hpp"
#include "shader/ShaderProgram.hpp"
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "physics/ElasticModel.hpp"
#include "physics/CollisionObjects.hpp"
#include "view/View.hpp"
#include "util/LoadMesh.hpp"

class CoordinateGridRender {
public:
    static constexpr std::array<double, 8> coordinateData = {-1, -1, -1, 1, 1, 1, 1, -1};
    double scale = 10;
    double spacing = 1;
    GLuint buffer, vao;
    ShaderProgram cgs;
    CoordinateGridRender(ShaderContext& context, double spacing):
        cgs(
            "resources/shaders/Grid.vert",
            "",
            "resources/shaders/Grid.frag",
            context
        ){
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &buffer);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
        glBufferData(GL_ARRAY_BUFFER, 8*sizeof(double), &coordinateData[0], GL_STATIC_DRAW);
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
    ElasticModel& m;
    ShaderProgram ems;
    ShaderProgram bbs;
    const unsigned restartIndex = 65535;
    bool drawBoundingBoxes=true;
    GLuint vertexBuffer, indexBuffer, vao;
    ElasticModelRender(ElasticModel& m, ShaderContext& context): m(m),
        ems(
            "resources/shaders/ElasticMesh.vert",
            "resources/shaders/ElasticMesh.geom",
            "resources/shaders/ElasticMesh.frag",
            context
        ),
        bbs(
            "resources/shaders/BoundingBox.vert",
            "resources/shaders/BoundingBox.geom",
            "resources/shaders/BoundingBox.frag",
            context
        )
    {
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &indexBuffer);
        glGenVertexArrays(1, &vao);

        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, m.vertexCount()*sizeof(double)*2, m.x2.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.triangleCount()*sizeof(unsigned int)*3,
            m.Te.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);
    }
    void draw() {
        ems.enable();
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m.vertexCount()*sizeof(double)*2, m.x2.data());
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glDrawElements(GL_TRIANGLES, 3*m.triangleCount(), GL_UNSIGNED_INT, nullptr);
        if(drawBoundingBoxes) {
            // glEnable(GL_PRIMITIVE_RESTART);
            bbs.enable();
            // glPrimitiveRestartIndex(restartIndex);
            std::vector<double> coords;
            // std::vector<std::array<unsigned int, 4>> indices(m.collisionObjects.size());
            unsigned i=0;
            for(auto& c: m.collisionObjects) {
                auto& b = c->boundingBox();
                coords.insert(coords.end(), {b.x0, b.y0, b.x0, b.y1, b.x1, b.y1, b.x1, b.y0});
                // indices.push_back({i, i+1, i+2, i+3, restartIndex});
                i += 5;
            }

            GLuint vb;
            glGenBuffers(1, &vb);
            // glGenBuffers(1, &ib);
            glBindBuffer(GL_ARRAY_BUFFER, vb);
            glBufferData(GL_ARRAY_BUFFER, coords.size()*sizeof(double), &coords[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
            // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
            // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int)*4,
            //     &indices[0][0], GL_STATIC_DRAW);
            // glDrawElements(GL_TRIANGLES, indices.size()*4, GL_UNSIGNED_INT, nullptr);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            // glDisable(GL_PRIMITIVE_RESTART);
            glDeleteBuffers(1, &vb);
            // glDeleteBuffers(1, &ib);
        }
        glBindVertexArray(0);
    }
};

ElasticModel selfCollisionTest() {
    double mu = 5;
    double lambda =  5;
    const std::vector<double> vertices =  {
        0, 0, 0, 1, 1, 0,
        2, 2, 2, 3, 3, 2,
    };
    const std::vector<double> k(2, mu);
    const std::vector<double> nu(2, lambda);
    const std::vector<double> M =  {1,1,1,1,1,1};
    const std::vector<std::array<unsigned,3>>indices =  {{0,1,2}, {3,4,5}};
    ElasticModel::ElasticModelType modelType = ElasticModel::ElasticModelType::INVERTIBLE_NEOHOOKEAN;
    ElasticModel em(vertices, indices, k, nu, M, modelType, (double)0.3);
    for(int i=0; i<6; i++) {
        em.x1(6+i) += 0.1;
    }
    em.kDamp = 0.1;
    em.dt = 0.01;
    return em;
};

ElasticModel ballTest() {
    double nuV = 0.49;
    double kV =  10.0;
    std::vector<double> vertices;
    std::vector<std::array<unsigned,3>>indices;
    loadMesh2D("resources/meshes/ball.obj", vertices, indices, 0, 4);
    const std::vector<double> k(indices.size(), kV);
    const std::vector<double> nu(indices.size(), nuV);
    const std::vector<double> M(vertices.size(), 1.0);
    ElasticModel::ElasticModelType modelType = ElasticModel::ElasticModelType::INVERTIBLE_NEOHOOKEAN;
    ElasticModel em(vertices, indices, k, nu, M, modelType, (double)0.3);
    for(int i=0; i<6; i++) {
        // em.x1(6+i) += 0.1;
    }
    // std::cout << em.x0 << std::endl;
    for(unsigned i=1; i<em.fExt.size(); i+=2)
        em.fExt[i] = -0.5;
    for(unsigned i=0; i<em.fExt.size(); i+=2)
        em.fExt[i] = -0.2;
    auto r = new Rectangle(10,1);
    em.collisionObjects.push_back(r);
    em.kDamp = 0.1;
    em.dt = 0.01;
    return em;
};

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);


    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui GLFW+OpenGL3 example", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    glewInit();
    // During init, enable debug output
    glEnable( GL_DEBUG_OUTPUT );
    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDebugMessageCallback( MessageCallback, 0 );

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 450";
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.00f, 0.f, 0.f, 1.00f);




    ElasticModel em = ballTest();
    ShaderContext context;
    ElasticModelRender emr(em, context);
    CoordinateGridRender cgr(context, 1.0);
    View view(window);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show a simple window.
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
        if (show_demo_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        // Rendering
        ImGui::Render();
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        em.implicitEulerStep();
        context.setViewMatrix(view.computeViewMatrix());
        cgr.draw();
        emr.draw();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
