//
// Created by fxzoroark on 09/01/23.
//

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
            Vec<3, double> v{};
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
            Vec<2, double> vt;
            iss >> vt[0] >> vt[1];
            texCoords.push_back(vt);
        }
        else if(key == "vn"){
            Vec<3, double> v{};
            iss >> v[0] >> v[1] >> v[2];
            verticesN.push_back(v);
        }

    }
    file.close();
    load_texture(filename + "_diffuse.tga", diffusemap );
    load_texture(filename + "_nm.tga", normalmap );
    load_texture(filename + "_spec.tga", specularmap );


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

std::vector<Vec<3, double>> Model::getFacePoints(int idx){
    std::vector<Vec<3, double>> vec;
    vec.push_back(vertices.at(std::get<0>(faces.at(idx))));
    vec.push_back(vertices.at(std::get<1>(faces.at(idx))));
    vec.push_back(vertices.at(std::get<2>(faces.at(idx))));
    return vec;
}
std::vector<Vec<3, double>> Model::getFaceNormals(int idx){
    std::vector<Vec<3, double>> vec;
    vec.push_back(verticesN.at(std::get<0>(faces.at(idx))));
    vec.push_back(verticesN.at(std::get<1>(faces.at(idx))));
    vec.push_back(verticesN.at(std::get<2>(faces.at(idx))));
    return vec;
}
std::vector<Vec<2, double>> Model::getTexturePoints(int idx) {
    std::vector<Vec<2, double>> vec;
    vec.push_back(texCoords.at(std::get<0>(textures.at(idx))));
    vec.push_back(texCoords.at(std::get<1>(textures.at(idx))));
    vec.push_back(texCoords.at(std::get<2>(textures.at(idx))));
    return vec;
}

TGAColor Model::getDiffuse(Vec<2, double> uv){
    return diffusemap.get(diffusemap.width() * uv.x(), diffusemap.height() - diffusemap.height() * uv.y());
}

float Model::getSpecular(Vec<2, double> uv){
    return specularmap.get(specularmap.width() * uv.x(), specularmap.height() - specularmap.height() * uv.y())[0] /1.f;
}

Vec<3, double> Model::getNormal(Vec<2, double> uv){
    TGAColor color = normalmap.get(normalmap.width() * uv.x(), normalmap.height() - normalmap.height() * uv.y());
    Vec<3, double> res;
    for (int i = 2; i >= 0; --i) {
        res[2-i] = (double)color[i]/255. * 2 - 1;
    }
    return res;
}