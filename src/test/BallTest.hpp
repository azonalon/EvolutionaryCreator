#include "physics/ElasticModel.hpp"
#include "util/LoadMesh.hpp"

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