#pragma once
#include <fstream>
#include <vector>
#include <cassert>
#include <array>

static void loadMesh2D(const char* file, std::vector<double>& vertices,
                        std::vector<std::array<unsigned, 3>>& indices,
                        double xoffset=0, double yoffset=0) {

    std::ifstream f;
    f.open(file);
    if(!f.good()) {
        fprintf(stderr, "File %s not found.", file);
        throw std::system_error();
    }
    std::string line;
    while(std::getline(f, line)) {
        if(line[1] != ' ') {
            continue;
        }
        else if (line[0] == 'f') {
            std::array<unsigned, 3> ind;
            sscanf(line.c_str(), "f %u// %u// %u//", &ind[0], &ind[1], &ind[2]);
            ind[0] -= 1;
            ind[1] -= 1;
            ind[2] -= 1;
            indices.push_back(ind);
        }
        else if (line[0] == 'v') {
            double x, y;
            sscanf(line.c_str(), "v %lf %lf %*f", &x, &y);
            vertices.push_back(x + xoffset);
            vertices.push_back(y + yoffset);
        }
    }
    f.close();
}
