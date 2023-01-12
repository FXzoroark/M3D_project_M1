//
// Created by fxzoroark on 09/01/23.
//

#ifndef PROJECT_MODEL_H
#define PROJECT_MODEL_H


#include <string>
#include <vector>
#include "geometry.h"

class Model {
    std::vector<Vec3<float>> vertices{};
    std::vector<std::tuple<int, int, int>> faces{};
public:
    explicit Model(const std::string& filename);
    std::vector<Vec3<float>> getFacePoints(int idx);
    int nbVerticies() const;
    int nbFaces() const;


};


#endif //PROJECT_MODEL_H
