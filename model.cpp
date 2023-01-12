//
// Created by fxzoroark on 09/01/23.
//

#include <fstream>
#include <iostream>
#include <tuple>
#include <sstream>
#include "model.h"

Model::Model(const std::string& filename) {
    std::ifstream file{filename};
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
            int f, t, n;
            while(iss >> f >> trash >> t >> trash >> n){
                fIds.push_back(--f);
            }
            faces.emplace_back(fIds[0], fIds[1], fIds[2]);
        }
    }
    file.close();
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
