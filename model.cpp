//
// Created by fxzoroark on 09/01/23.
//

#include <fstream>
#include <iostream>
#include <tuple>
#include <sstream>
#include "model.h"

Model::Model(const std::string& filename) {
    std::ifstream file{filename + ".obj"};
    std::string line;
    std::string key;
    char trash;
    while(std::getline(file, line)){
        std::istringstream iss(line);
        iss >> key;
        if(key == "v"){
            Vec3<float> v{};
            iss >> v[0] >> v[1] >> v[2];
            vertices.push_back(v);
        }
        else if(key == "f"){
            std::vector<int> fIds;
            std::vector<int> tIds;
            int f, t, n;
            while(iss >> f >> trash >> t >> trash >> n){
                fIds.push_back(--f);
                tIds.push_back(--t);
            }
            faces.emplace_back(fIds[0], fIds[1], fIds[2]);
            textures.emplace_back(tIds[0], tIds[1], tIds[2]);
        }
        else if(key == "vt"){
            Vec2<float> vt{};
            iss >> vt[0] >> vt[1];
            texCoords.push_back(vt);
        }
        else if(key == "vn"){
            Vec3<float> v{};
            iss >> v[0] >> v[1] >> v[2];
            verticesN.push_back(v);
        }

    }
    file.close();
    load_texture(filename + "_diffuse.tga", diffusemap );
}

void Model::load_texture(const std::string& filename, TGAImage &img) {
    std::cerr << "texture file " << filename << " loading " << (img.read_tga_file(filename) ? "ok" : "failed") << std::endl;
}

int Model::nbVerticies() const{
    return (int)vertices.size();
}

int Model::nbFaces() const{
    return (int)faces.size();
}

std::vector<Vec3<float>> Model::getFacePoints(int idx){
    std::vector<Vec3<float>> vec;
    vec.push_back(vertices.at(std::get<0>(faces.at(idx))));
    vec.push_back(vertices.at(std::get<1>(faces.at(idx))));
    vec.push_back(vertices.at(std::get<2>(faces.at(idx))));
    return vec;
}
std::vector<Vec3<float>> Model::getFaceNormals(int idx){
    std::vector<Vec3<float>> vec;
    vec.push_back(verticesN.at(std::get<0>(faces.at(idx))));
    vec.push_back(verticesN.at(std::get<1>(faces.at(idx))));
    vec.push_back(verticesN.at(std::get<2>(faces.at(idx))));
    return vec;
}
std::vector<Vec2<float>> Model::getTexturePoints(int idx) {
    std::vector<Vec2<float>> vec;
    vec.push_back(texCoords.at(std::get<0>(textures.at(idx))));
    vec.push_back(texCoords.at(std::get<1>(textures.at(idx))));
    vec.push_back(texCoords.at(std::get<2>(textures.at(idx))));
    return vec;
}