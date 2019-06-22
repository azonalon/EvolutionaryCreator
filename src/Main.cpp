#include <stdio.h>
#include <Eigen/Dense>
#include "render/OpenGLContext.hpp"
#include "imgui/imgui.h"
#include "render/Render.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "physics/CollisionObjects.hpp"
#include "physics/ElasticModel.hpp"
#include "physics/Phonons.hpp"
#include "shader/ShaderProgram.hpp"
#include "util/LoadMesh.hpp"
#include "view/View.hpp"

using namespace std;

ElasticModelRender *fourierTestEmr(Phonons* ph, ShaderContext& context) {
  int width, height, nrChannels;
  unsigned char *data = stbi_load("resources/textures/worldmap.png", &width,
                                  &height, &nrChannels, 4);
  Eigen::ArrayXd uv = ph->model->x0;
  uv = uv / uv.maxCoeff();
  // std::cout << uv << std::endl;
  ElasticModelRender *emr =
      new ElasticModelRender(ph->model, context, data, width, height, &uv[0]);
  return emr;
}

Phonons *fourierTest() {
  double mu = 10;
  double lambda = 10;
  unsigned N1=3, N2=3;
  std::vector<double> vertices;
  std::vector<std::array<unsigned, 3>> indices;
  for(unsigned i=0; i<N2; i++) {
    for(unsigned j=0; j<N1; j++) {
      vertices.push_back(j);
      vertices.push_back(i);
      if(i<N2-1 && j < N1-1) {
        indices.push_back({i*N1 + j, i*N1 + j+1  , (i+1)*N1 + j + 1});
        indices.push_back({i*N1 + j, (i+1)*N1 + j, (i+1)*N1 + j + 1});
      } 
    }
  }
  const std::vector<double> k(indices.size(), mu);
  const std::vector<double> nu(indices.size(), lambda);
  const std::vector<double> M(vertices.size(), 0.1);
  ElasticModel::ElasticModelType modelType =
      ElasticModel::ElasticModelType::STABLE_NEOHOOKEAN;
  ElasticModel *em =
      new ElasticModel(vertices, indices, k, nu, M, modelType, (double)0.3);
  Phonons* ph = new Phonons(em, 0, N1, N2);
  em->kDamp = 0.0;
  em->dt = 0.01;
  return ph;
};

int main(int, char **) {
  // Setup window
  OpenGLContext contextGL;
  contextGL.init();
  bool stepping = true;
  int seed=59;

  srand(seed);
  Phonons *ph = fourierTest();
  ElasticModel* em = ph->model;
  ShaderContext context;
  ElasticModelRender* emr = fourierTestEmr(ph, context);
  CoordinateGridRender cgr(context, 1.0);

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
  float mu = 30;//em->mu[0];
  float lambda = 30;//em->lambda[0];
  float kFluid = 0.5;//em->kFluid;
  float kDamp = 0.05;//em->kDamp;
  do  {
    {
      ImGui::Text("Stepper interface");  // Display some text (you can use a
                                         // format string too)
      ImGui::Checkbox("Continue stepping", &stepping);
      ImGui::SliderInt("Random Seed", &seed, 0, 100);
      srand(seed);

      ImGui::SliderFloat("mu", &mu, 0, 100);
      ImGui::SliderFloat("k Fluid", &kFluid, 0, 2);
      ImGui::SliderFloat("lambda", &lambda, 0, 100);
      ImGui::SliderFloat("k internal damping", &kDamp, 0, 2);
      if (em->mu[0] != mu) {
        for (auto &x : em->mu) x = mu;
      }
      if (em->lambda[0] != lambda) {
        for (auto &x : em->lambda) x = lambda;
      }
      em->kFluid = kFluid;
      em->kDamp = kDamp;

      if (ImGui::Button("One step")) {
        lines.clear();
        em->implicitEulerStep();
        t += em->dt;
      }
      if (ImGui::Button("Reset")) {
        // delete em;
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


    if (stepping) {
      lines.clear();
      try {
        ph->update(t);
        em->implicitEulerStep();
        t += em->dt;
      } catch(const std::runtime_error& e) {
        std::cout << "\nError during stepping " << e.what() << std::endl;
        stepping = false;
      }
    }
    cgr.draw();
    context.setViewMatrix(contextGL.view.computeViewMatrix());
    emr->draw();
  } while (contextGL.nextFrame());

  delete em;
  contextGL.destroy();
  return 0;
}
