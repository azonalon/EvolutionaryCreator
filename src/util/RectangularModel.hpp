ElasticModel* rectangularModel() {
  double mu = 10;
  double lambda = 10;
  unsigned N1=10, N2=10;
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
  for(unsigned i=N2; i<2*N2; i++) {
    for(unsigned j=0; j<N1; j++) {
      vertices.push_back(j - 0);
      vertices.push_back(i - 0);
      if(i<2*N2-1 && j < 2*N1-1) {
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
  em->x1 = em->x0 + 0.5;
  CollisionObject* rect = new Rectangle(200, 1);
  rect->translate(0, -3);
  em->collisionObjects.push_back(rect);
  for(int i=0; i<em->v.size(); i+=2)
  {
    em->v[i] = 0;
  }
  em->kDamp = 0.0;
  em->dt = 0.01;
  return em;
};