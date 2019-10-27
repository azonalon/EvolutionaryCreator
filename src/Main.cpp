#include <stdio.h>
#include <eigen3/Eigen/Dense>
#include "render/OpenGLContext.hpp"
#include "imgui/imgui.h"
#include "render/Render.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "physics/CollisionObjects.hpp"
#include "physics/ElasticModel.hpp"
#include "shader/ShaderProgram.hpp"
#include "util/LoadMesh.hpp"
#include "view/View.hpp"

using namespace std;

ElasticModelRender *constructElasticModelRender(ElasticModel* model, ShaderContext& context) {
  int width, height, nrChannels;
  unsigned char *data = stbi_load("resources/textures/worldmap.png", &width,
                                  &height, &nrChannels, 4);
  Eigen::ArrayXd uv = model->x0;
  uv = uv / uv.maxCoeff();
  // std::cout << uv << std::endl;
  ElasticModelRender *emr =
      new ElasticModelRender(model, context, data, width, height, &uv[0]);
  return emr;
}

ElasticModel* rectangularModel() {
  double mu = 10;
  double lambda = 10;
  unsigned N1=10, N2=10;
  std::vector<double> vertices;
  std::vector<std::array<unsigned, 3>> indices;
  loadMesh2D("resources/meshes/ramp.obj", vertices, indices, 0, 0);
  const std::vector<double> k(indices.size(), mu);
  const std::vector<double> nu(indices.size(), lambda);
  const std::vector<double> M(vertices.size(), 0.1);
  ElasticModel::ElasticModelType modelType =
      ElasticModel::ElasticModelType::STABLE_NEOHOOKEAN;
  ElasticModel *em =
      new ElasticModel(vertices, indices, k, nu, M, modelType, (double)0.3);
  em->x1 = em->x0 + 0.5;
  CollisionObject* rect = new Rectangle(200, 1);
  rect->translate(0, -1);
  em->collisionObjects.push_back(rect);
  for(int i=0; i<em->v.size(); i+=2)
  {
    em->v[i] = 0;
  }
  em->kDamp = 0.0;
  em->dt = 0.1;
  return em;
};
void printCursorPos(OpenGLContext context) {
  ImVec2 pos = ImGui::GetMousePos();
  double w = context.view.w;
  double h = context.view.h;
  glm::vec4 v((-1+2*pos.x/w), (1-2*pos.y/h), 0, 1);
  v = glm::inverse(context.view.shaderContext->viewMatrix)*v;
  // drawList->AddText(ImVec2((1+v.x)*w/2, (1-v.y)*h/2), 
  //                           IM_COL32(255, 0, 255, 255), s.str().c_str());
  ImGui::Text("Cursor position: %g, %g (screen) | %g, %g (world)", pos.x, pos.y, v.x, v.y);
}

int main(int, char **) {
  // Setup window
  OpenGLContext contextGL;
  contextGL.init();
  bool stepping = false;
  int seed=59;

  srand(seed);
  ElasticModel* em = rectangularModel();

  ShaderContext context;
  contextGL.view.shaderContext=&context;

  ElasticModelRender* emr = constructElasticModelRender(em, context);
  CoordinateGridRender cgr(context, 1.0);
  ImFont* font = ImGui::GetFont();
  ImDrawList* drawList = ImGui::GetOverlayDrawList();

  std::vector<std::pair<Eigen::ArrayXf, double>> lines;
  auto printCollisionList = [&](const std::vector<std::array<unsigned, 3>>& list) {
    if(list.size() == 0) {
      ImGui::Text("No Collisions");
      return;
    }
    ImGui::Text("%d Collisions", list.size());
    for(auto& triplet: list) {
      ImGui::Text("%d %d %d", triplet[0]/2, triplet[1]/2, triplet[2]/2);
    }
  };
  // em->lineSearchHook = [&](auto *s, double alpha) {
  //   unsigned N = 300;
  //   Eigen::ArrayXf values(N);
  //   Eigen::ArrayXd vOld = s->v;
  //   Eigen::ArrayXd x(s->x0);
  //   auto alphas = Eigen::ArrayXd::LinSpaced(N, -2, 2);
  //   for (unsigned i = 0; i < N; i++) {
  //     x = s->x0 + alphas[i] * s->dn;
  //     s->g = 0;
  //     values(i) = s->computeOptimizeGradient(x, s->g);
  //   }
  //   lines.push_back(make_pair(values, alpha));
  //   s->v = vOld;
  // };
  double t = 0;

  // Main loop
  float mu = 80;//em->mu[0];
  float lambda = 80;//em->lambda[0];
  float kFluid = 5;//em->kFluid;
  float kDamp = 0.05;//em->kDamp;
  float newtonAccuracy = 0.01;//em->kDamp;
  float fExt = 0.04;
  float muFriction = 1;
  float dt = 0.1;
  do  {
    {
      ImGui::Text("Stepper interface");  // Display some text (you can use a
                                         // format string too)
      ImGui::Checkbox("Continue stepping", &stepping);
      ImGui::SliderInt("Random Seed", &seed, 0, 100);
      srand(seed);

      ImGui::SliderFloat("mu", &mu, 0, 10);
      ImGui::SliderFloat("k Fluid", &kFluid, 0, 10);
      ImGui::SliderFloat("lambda", &lambda, 0, 10);
      ImGui::SliderFloat("k internal damping", &kDamp, 0, 2);
      ImGui::SliderFloat("gravity", &fExt, -2, 2);
      ImGui::SliderFloat("newton accuracy", &newtonAccuracy, 0.0001, 0.1);
      ImGui::SliderFloat("mu Friction", &muFriction, 0.0, 20);
      ImGui::SliderFloat("dt", &dt, 0.01, 1);
      if (em->mu[0] != mu) {
        for (auto &x : em->mu) x = mu;
      }
      if (em->lambda[0] != lambda) {
        for (auto &x : em->lambda) x = lambda;
      }
      em->kFluid = kFluid;
      em->kDamp = kDamp;
      em->newtonAccuracy = newtonAccuracy;
      em->fExt = 0;
      em->dt = dt;
      muSelfFriction = muFriction;
      for(int i=0; i < 0; i++) {
        em->setSurfaceForce(0, i, 0.1);
      }
      for(int i=1; i<em->fExt.size(); i+=2) 
      {
        em->fExt[i] += fExt;
      }

      if (ImGui::Button("One step")) {
        lines.clear();
        em->implicitEulerStep();
        t += em->dt;
      }
      if (ImGui::Button("Reset")) {
        // delete em;
        em = rectangularModel();
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
      printCollisionList(em->selfCollisionList);
    }


    if (stepping) {
      lines.clear();
      try {
        // ph->update(t);
        em->implicitEulerStep();
        t += em->dt;
      } catch(const std::runtime_error& e) {
        std::cout << "\nError during stepping " << e.what() << std::endl;
        stepping = false;
      }
    }
    cgr.draw();
    context.setViewMatrix(contextGL.view.computeViewMatrix());
    printCursorPos(contextGL);
    emr->draw();
    emr->drawIndices(context, contextGL);
  } while (contextGL.nextFrame());

  delete em;
  contextGL.destroy();
  return 0;
}
