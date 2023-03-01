//
// Created by fxzoroark on 09/01/23.
//

#ifndef PROJECT_MODEL_H
#define PROJECT_MODEL_H


#include <string>
#include "vector"
#include "vec.h"
#include "tgaimage.h"

class Model {
    std::vector<Vec<3, double>> vertices;
    std::vector<Vec<3, double>> verticesN;
    std::vector<std::tuple<int, int, int>> faces;
    std::vector<Vec<2, double>> texCoords;
    std::vector<std::tuple<int, int, int>> textures;

    TGAImage diffusemap{};         // diffuse color texture
    TGAImage normalmap{};          // normal texture of the object
    TGAImage specularmap{};        // specular texture of the object

    static void load_texture(const std::string& filename, TGAImage &img);

public:
    explicit Model(const std::string& filename);
    std::vector<Vec<3, double>> getFacePoints(int idx);
    std::vector<Vec<3, double>> getFaceNormals(int idx);
    std::vector<Vec<2, double>> getTexturePoints(int idx);
    TGAColor getDiffuse(Vec<2, double> uv);
    Vec<3, double> getNormal(Vec<2, double> uv);

    int nbVerticies() const;
    int nbFaces() const;

    float getSpecular(Vec<2, double> uv);
};


#endif //PROJECT_MODEL_H
