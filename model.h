//
// Created by fxzoroark on 09/01/23.
//

#ifndef PROJECT_MODEL_H
#define PROJECT_MODEL_H


#include <string>
#include <vector>
#include "geometry.h"
#include "tgaimage.h"

class Model {
    std::vector<Vec3<float>> vertices{};
    std::vector<Vec3<float>> verticesN{};
    std::vector<std::tuple<int, int, int>> faces{};
    std::vector<Vec2<float>> texCoords{};
    std::vector<std::tuple<int, int, int>> textures{};

    TGAImage diffusemap{};         // diffuse color texture

    static void load_texture(const std::string& filename, TGAImage &img);

public:
    explicit Model(const std::string& filename);
    std::vector<Vec3<float>> getFacePoints(int idx);
    std::vector<Vec3<float>> getFaceNormals(int idx);
    std::vector<Vec2<float>> getTexturePoints(int idx);
    int nbVerticies() const;
    int nbFaces() const;
    TGAImage& getDiffuse() {return diffusemap;};

};


#endif //PROJECT_MODEL_H
