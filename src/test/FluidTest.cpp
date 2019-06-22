#include "render/OpenGLContext.hpp"
#include "render/Render.hpp"
#include "render/FlowRender.hpp"
#include "physics/FluidDynamics.hpp"
#include "Eigen/Dense"
#include "render/StaticObjectRender.hpp"
#include <random>

int main(void) {
  std::random_device rd{};
  std::mt19937 gen{rd()};
  double vmin=0, vmax = 20;

  // values near the mean are the most likely
  // standard deviation affects the dispersion of generated values from the mean
  std::normal_distribution<> dx{-2, 0.0};
  std::normal_distribution<> dy{0, 0};
  unsigned nSurface = 20;
  unsigned n = 100;
  unsigned nVortices = 0;
  Eigen::ArrayXXd X(n, n);
  Eigen::ArrayXXd Y(n, n);
  Eigen::ArrayXXd U(n, n);
  Eigen::ArrayXXd V(n, n);
  std::vector<GLubyte> texture(n * n * 4);
  Eigen::ArrayXd surface(2 * nSurface);
  std::vector<std::vector<const double*>> surfaceList(1);
  for (unsigned i = 0; i < nSurface; i++) {
    surface[2 * i + 0] = cos((double)i / (double)nSurface * 2 * M_PI);
    surface[2 * i + 1] = sin((double)i / (double)nSurface * 2 * M_PI);
    surfaceList[0].push_back(&surface(2 * i));
  }
  FluidDynamicSolver S(surfaceList);
  S.uInfinity = 5;
  S.vInfinity =  5;
  for (unsigned j = 0; j < nVortices; j++) {
    S.vortices.insert(S.vortices.begin(), {dx(gen), dy(gen)});
    S.uVortices.insert(S.uVortices.begin(), {dx(gen), dy(gen)});
    S.uVorticesOld.insert(S.uVorticesOld.begin(), {dx(gen), dy(gen)});
    S.velocities.insert(S.velocities.begin(), {5});
  }

  for (unsigned i = 0; i < n; i++) {
    for (unsigned j = 0; j < n; j++) {
      double x = 12 * ((double)i - n / 2.0) / n;
      double y = 12 * ((double)j - n / 2.0) / n;
      X(i, j) = x;
      Y(i, j) = y;
      U(i, j) = 0;
      V(i, j) = 0;
      x = x + 0.5;
      ColorMapJet::COLOUR color = ColorMapJet::GetColour(
          U(i, j) * U(i, j) + V(i, j) * V(i, j), vmin, vmax);
      texture[4 * i * n + 4 * j + 0] = color.r * 255;
      texture[4 * i * n + 4 * j + 1] = color.g * 255;
      texture[4 * i * n + 4 * j + 2] = color.b * 255;
      texture[4 * i * n + 4 * j + 3] = 255;
    }
  }
  TextureBounds bounds{X(0, 0), Y(0, 0), X(n - 1, n - 1),
                       Y(n - 1, n - 1)};  // x0, y0, x1, y1
  OpenGLContext contextGL;
  contextGL.init();
  ShaderContext contextS;
  FlowRender fr(contextS, bounds, n, n);
  StaticObjectRender sr(contextS, surface.data(), nSurface);
  CoordinateGridRender cgr(contextS, 1.0);
  // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  int steps = 0;
  contextGL.view.zoom = 3;
  float tau=S.tau, kDiff = S.kDiff, sigma=S.sigma, theta=0, sInf=3, kDecay=S.kDecay, dt=S.dt,
      deltaSlip=S.deltaSlip;
  int nVortex=0;

  do {
    cgr.draw();
    sr.draw();
    fr.draw(&texture[0], X, Y, U, V, n, n);
    fr.drawVortices(S.vortices, S.velocities);
    S.step();
    S.computeFlow(X, Y, U, V);
    ImGui::Text("Control parameters");  // Display some text (you can use a
                                        // format string too)
    ImGui::SliderFloat("tau", &tau, -10, 10);
    ImGui::SliderFloat("k diffuse", &kDiff, 0, 2);
    ImGui::SliderFloat("sigma", &sigma, 0, 2);
    ImGui::SliderFloat("theta", &theta, 0, M_PI*2);
    ImGui::SliderInt("vortex emission", &nVortex, 0, 100);
    ImGui::SliderFloat("sInf", &sInf, 0, 30);
    ImGui::SliderFloat("delta slip", &deltaSlip, 0, 1);
    ImGui::SliderFloat("kDecay", &kDecay, 0, 100);
    ImGui::SliderFloat("dt", &dt, 0, 0.1);
    ImGui::Text("Vortex count %d", S.vortices.size()/2);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    S.tau = tau; S.kDiff = kDiff; S.sigma = sigma; S.uInfinity = sInf*cos(theta); 
    S.vInfinity = sInf*sin(theta); S.kDecay = kDecay; S.dt = dt; S.deltaSlip = deltaSlip;
    for(int i=0; i<nVortex; i++) {
      S.emitVortex(-5.0, 0.0, 0.0, 0.0, S.tau*dt*100);
    }
    // std::cout << S.vortices.size() << std::endl;
    for (unsigned i = 0; i < n; i++) {
      for (unsigned j = 0; j < n; j++) {
        // TODO: calculate U and V from vortex particles
        ColorMapJet::COLOUR color = ColorMapJet::GetColour(
            U(i, j) * U(i, j) + V(i, j) * V(i, j), vmin, vmax);
        texture[4 * i * n + 4 * j + 0] = color.r * 255;
        texture[4 * i * n + 4 * j + 1] = color.g * 255;
        texture[4 * i * n + 4 * j + 2] = color.b * 255;
        texture[4 * i * n + 4 * j + 3] = 100;
      }
    }
    contextS.setViewMatrix(contextGL.view.computeViewMatrix());
    steps++;
  } while (contextGL.nextFrame() && steps < 50000);
}