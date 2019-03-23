#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <Eigen/Dense>
#include "imgui/imgui.h"
#include "render/Render.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "physics/CollisionObjects.hpp"
#include "physics/ElasticModel.hpp"
#include "shader/ShaderProgram.hpp"
#include "ui/ImGuiGlfw.hpp"
#include "ui/ImGuiOpengl.hpp"
#include "util/LoadMesh.hpp"
#include "view/View.hpp"

using namespace std;

ElasticModelRender *fourierTestEmr(Phonons* ph, ShaderContext& context) {
  int width, height, nrChannels;
  unsigned char *data = stbi_load("resources/textures/worldmap.png", &width,
                                  &height, &nrChannels, 4);
  Eigen::ArrayXd uv = ph->model->x0;
  uv = uv / uv.maxCoeff();
  std::cout << uv << std::endl;
  ElasticModelRender *emr =
      new ElasticModelRender(ph->model, context, data, width, height, &uv[0]);
  return emr;
}

Phonons *fourierTest() {
  double mu = 40;
  double lambda = 40;
  unsigned N1=10, N2=10;
  std::vector<double> vertices;
  std::vector<std::array<unsigned, 3>> indices;
  for(unsigned i=0; i<N1; i++) {
    for(unsigned j=0; j<N2; j++) {
      vertices.push_back(i);
      vertices.push_back(j);
      if(i<N1-1 && j < N2-1) {
        indices.push_back({j*N1 + i, j*N1 + i+1, (j+1)*N1 + i + 1});
        indices.push_back({j*N1 + i, (j+1)*N1 + i, (j+1)*N1 + i + 1});
      } 
    }
  }
  const std::vector<double> k(indices.size(), mu);
  const std::vector<double> nu(indices.size(), lambda);
  const std::vector<double> M(vertices.size(), 1.0);
  ElasticModel::ElasticModelType modelType =
      ElasticModel::ElasticModelType::STABLE_NEOHOOKEAN;
  ElasticModel *em =
      new ElasticModel(vertices, indices, k, nu, M, modelType, (double)0.3);
  Phonons* ph = new Phonons(em, 0, N1, N2);
  em->kDamp = 0.3;
  em->dt = 0.01;
  return ph;
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

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char *glsl_version = "#version 450";
  ImGui_ImplOpenGL3_Init(glsl_version);

  ImGui::StyleColorsDark();
  bool stepping = false;
  int seed=59;
  ImVec4 clear_color = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);

  srand(seed);
  Phonons *ph = fourierTest();
  ElasticModel* em = ph->model;
  ShaderContext context;
  ElasticModelRender* emr = fourierTestEmr(ph, context);
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
    glfwPollEvents();

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
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
        ph = fourierTest();
        em = ph->model;
        emr->m = em;
        t = 0;
      }
      ImGui::SameLine();
      ImGui::Text("t = %g", t);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

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
        ph->update(t);
        em->implicitEulerStep();
        t += em->dt;
      } catch(const std::runtime_error& e) {
        std::cout << "\nError during stepping " << e.what() << std::endl;
        stepping=false;
      }
    }
    context.setViewMatrix(view.computeViewMatrix());
    // cgr.draw();
    emr->draw();
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
