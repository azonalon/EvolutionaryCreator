#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <Eigen/Dense>
#include "imgui/imgui.h"
#include "physics/CollisionObjects.hpp"
#include "physics/ElasticModel.hpp"
#include "shader/ShaderProgram.hpp"
#include "ui/ImGuiGlfw.hpp"
#include "ui/ImGuiOpengl.hpp"
#include "util/LoadMesh.hpp"
#include "view/View.hpp"

using namespace std;
class CoordinateGridRender {
 public:
  static constexpr std::array<double, 8> coordinateData = {-1, -1, -1, 1,
                                                           1,  1,  1, -1};
  double scale = 10;
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
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(double), &coordinateData[0],
                 GL_STATIC_DRAW);
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
  ElasticModel *m;
  ShaderProgram ems;
  ShaderProgram bbs;
  const unsigned restartIndex = 65535;
  bool drawBoundingBoxes = true;
  GLuint vertexBuffer, indexBuffer, vao;
  ElasticModelRender(ElasticModel *m, ShaderContext &context)
      : m(m),
        ems("resources/shaders/ElasticMesh.vert",
            "resources/shaders/ElasticMesh.geom",
            "resources/shaders/ElasticMesh.frag", context),
        bbs("resources/shaders/BoundingBox.vert",
            "resources/shaders/BoundingBox.geom",
            "resources/shaders/BoundingBox.frag", context) {
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &indexBuffer);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m->vertexCount() * sizeof(double) * 2,
                 m->x2.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 m->triangleCount() * sizeof(unsigned int) * 3, m->Te.data(),
                 GL_STATIC_DRAW);
    glBindVertexArray(0);
  }
  void draw() {
    ems.enable();
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m->vertexCount() * sizeof(double) * 2,
                    m->x2.data());
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements(GL_TRIANGLES, 3 * m->triangleCount(), GL_UNSIGNED_INT,
                   nullptr);
    if (drawBoundingBoxes) {
      // glEnable(GL_PRIMITIVE_RESTART);
      bbs.enable();
      // glPrimitiveRestartIndex(restartIndex);
      std::vector<double> coords;
      // std::vector<Eigen::Affine2d, Eigen::aligned_allocator<Eigen::Affine2d>> transforms;
      // std::vector<std::array<unsigned int, 4>>
      // indices(m->collisionObjects.size());
      for (auto &co : m->collisionObjects) {
        auto bb = co->boundingBoxUntransformed();
        Eigen::Vector2d a(bb.x0, bb.y0),b(bb.x0,bb.y1),c(bb.x1,bb.y0),d(bb.x1, bb.y1);
        a = co->aux.inverse()*a; b = co->aux.inverse()*b; c = co->aux.inverse()*c; d = co->aux.inverse()*d;
        coords.insert(coords.end(),
                      {a.x(), a.y(), b.x(), b.y(), c.x(), c.y(), b.x(), b.y(), c.x(), c.y(), d.x(), d.y()});
        // indices.push_back({i, i+1, i+2, i+3, restartIndex});
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
      // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
      // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned
      // int)*4,
      //     &indices[0][0], GL_STATIC_DRAW);
      // glDrawElements(GL_TRIANGLES, indices.size()*4, GL_UNSIGNED_INT,
      // nullptr);
      glDrawArrays(GL_TRIANGLES, 0, coords.size()/2);
      // glDisable(GL_PRIMITIVE_RESTART);
      glDeleteBuffers(1, &vb);
      // glDeleteBuffers(1, &ib);
    }
    glBindVertexArray(0);
  }
};

ElasticModel *selfCollisionTest() {
  return nullptr;
};

ElasticModel *ballTest() {
  double mu = 40;
  double lambda = 40;
  std::vector<double> vertices;
  std::vector<std::array<unsigned, 3>> indices;
  loadMesh2D("resources/meshes/ball.obj", vertices, indices, 0, 4);
  const std::vector<double> k(indices.size(), mu);
  const std::vector<double> nu(indices.size(), lambda);
  const std::vector<double> M(vertices.size(), 1.0);
  ElasticModel::ElasticModelType modelType =
      ElasticModel::ElasticModelType::STABLE_NEOHOOKEAN;
  ElasticModel *em =
      new ElasticModel(vertices, indices, k, nu, M, modelType, (double)0.3);
  // em->x0 = Eigen::ArrayXd::Random(em->x0.size()) / 2.0 + 2;
  // em->x0 = em->x0 + 5;
  // em->x1 = em->x0;
  // em->x2 = em->x0;
  // em->v = Eigen::ArrayXd::Random(em->x0.size())/2.0 + 2;
  // std::cout << "Initial positions: {";
  // for (int i = 0; i < em->x0.size(); i++) cout << vertices[i] << ", ";
  // std::cout << "}" << std::endl;
  // std::cout << "Random positions: {";
  // for (int i = 0; i < em->x0.size(); i++) cout << em->x0[i] << ", ";
  // std::cout << "}" << std::endl;
  // std::cout << em.x0 << std::endl;
  for (unsigned i = 1; i < em->fExt.size(); i += 2) em->fExt[i] = -0.2;
  for (unsigned i = 0; i < em->fExt.size(); i += 2) em->fExt[i] = -0.0;
  auto r = new Rectangle(15, 1);
  auto r1 = new Rectangle(15, 1);
  // r->aux = r->aux.rotate(1).translate(Eigen::Vector2d(0.0, 0.0));
  r->rotate(M_PI/4);
  r->translate(0, -3);
  r1->rotate(-M_PI/4);
  r1->translate(0, -3);
  em->collisionObjects.push_back(r);
  em->collisionObjects.push_back(r1);
  em->kDamp = 0.3;
  em->dt = 0.01;
  return em;
};

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                GLenum severity, GLsizei length,
                                const GLchar *message, const void *userParam) {
  fprintf(stderr,
          "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
          (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity,
          message);
}

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char **) {
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) return 1;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glEnable(GL_MULTISAMPLE);

  GLFWwindow *window =
      glfwCreateWindow(1280, 720, "ImGui GLFW+OpenGL3 example", NULL, NULL);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // Enable vsync
  glewInit();
  // During init, enable debug output
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDebugMessageCallback(MessageCallback, 0);

  // Setup Dear ImGui binding
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard
  // Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable
  // Gamepad Controls

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char *glsl_version = "#version 450";
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Setup style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can
  // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
  // them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
  // need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please
  // handle those errors in your application (e.g. use an assertion, or display
  // an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored
  // into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
  // ImGui_ImplXXXX_NewFrame below will call.
  // - Read 'misc/fonts/README.txt' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);

  bool stepping = false;
  int seed=59;
  ImVec4 clear_color = ImVec4(0.00f, 0.f, 0.f, 1.00f);

  srand(seed);
  ElasticModel *em = ballTest();
  ShaderContext context;
  ElasticModelRender emr(em, context);
  CoordinateGridRender cgr(context, 1.0);
  View view(window);

  std::vector<std::pair<Eigen::ArrayXf, double>> lines;
  em->lineSearchHook = [&](auto *s, double alpha) {
    unsigned N = 300;
    Eigen::ArrayXf values(N);
    s->g = 0;
    Eigen::ArrayXd x(s->x0);
    auto alphas = Eigen::ArrayXd::LinSpaced(N, 0, 2);
    for (unsigned i = 0; i < N; i++) {
      x = s->x0 + alphas[i] * s->dn;
      values(i) = s->computeOptimizeGradient(x, s->g);
    }
    lines.push_back(make_pair(values, alpha));
  };
  double t = 0;

  // Main loop
  float mu = em->mu[0];
  float lambda = em->lambda[0];
  while (!glfwWindowShouldClose(window)) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    // your main application.
    // - When io.WantCaptureKut data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them
    // from your application based on those two flags.
    glfwPollEvents();

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show a simple window.
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets
    // automatically appears in a window called "Debug".
    {
      // static float f = 0.0f;
      // static int counter = 0;
      ImGui::Text("Stepper interface");  // Display some text (you can use a
                                         // format string too)
      ImGui::Checkbox("Continue stepping", &stepping);
      ImGui::SliderInt("Random Seed", &seed, 0, 100);
      srand(seed);

      ImGui::SliderFloat("mu", &mu, 0, 100);
      ImGui::SliderFloat("lambda", &lambda, 0, 100);
      if (em->mu[0] != mu) {
        for (auto &x : em->mu) x = mu;
      }
      if (em->lambda[0] != lambda) {
        for (auto &x : em->lambda) x = lambda;
      }

      if (ImGui::Button("One step")) {
        lines.clear();
        em->implicitEulerStep();
        t += em->dt;
      }
      if (ImGui::Button("Reset")) {
        delete em;
        em = ballTest();
        emr.m = em;
        t = 0;
      }
      ImGui::SameLine();
      ImGui::Text("t = %g", t);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

      // ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset,
      // "avg 0.0", -1.0f, 1.0f, ImVec2(0,80));
      // ImGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL,
      // 0.0f, 1.0f, ImVec2(0,80));

      // struct Funcs
      // {
      //     static float Sin(double i) { return sinf(i); }
      //     static float Saw(double i) { return (int(i)); }
      // };
      static int func_type = 0, display_count = 70;
      ImGui::Separator();
      ImGui::PushItemWidth(100);
      ImGui::Combo("func", &func_type, "Sin\0Saw\0");
      ImGui::PopItemWidth();
      ImGui::SameLine();
      ImGui::SliderInt("Sample count", &display_count, 1, 400);
      // Eigen::ArrayXf X = Eigen::ArrayXf::LinSpaced(display_count, 0, 10);
      // X = X.unaryExpr(func_type == 0? &Funcs::Sin: Funcs::Saw);
      for (auto &p : lines) {
        char s[30];
        sprintf(s, "alpha=%g", p.second);
        ImGui::PlotLines("Lines", &(p.first)[0], p.first.size(), 0, s, FLT_MAX,
                         FLT_MAX, ImVec2(400, 200));
      }
      ImGui::Separator();
    }

    // Rendering
    ImGui::Render();

    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    if (stepping) {
      lines.clear();
      try {
        em->implicitEulerStep();
        t += em->dt;
      } catch(const std::runtime_error& e) {
        std::cout << "\nError during stepping " << e.what() << std::endl;
        stepping=false;
      }
    }
    context.setViewMatrix(view.computeViewMatrix());
    // cgr.draw();
    emr.draw();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
  }

  // Cleanup
  delete em;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
