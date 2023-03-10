//
// Created by fxzoroark on 12/01/23.
//

#include <iostream>
#include "draw.h"

void Draw::line(int x0, int y0, int x1, int y1, TGAColor color) {
    // 1er
    /*
    for(float t = 0.; t < 1.; t+=.1){
        //Coordonées barycentriques
        int x = x0*(1.-t) + x1*t;
        int y = y0*(1.-t) + y1*t;
        image.set(x, y, color);
    }
     */
    // 2eme
    /*
    for(int x=x0; x<=x1; x++){
        float t = (x-x0)/static_cast<float>(x1-x0);
        int y = y0 + (y1-y0)*t;
        image.set(x, y, color);
    }
     */
    // 3eme
    /*
    bool steep = false;
    if(std::abs(x0-x1)<std::abs(y0-y1)){
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if(x0>x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for(int x=x0; x<=x1; x++){
        float t = (x-x0)/static_cast<float>(x1-x0);
        int y = y0 + (y1-y0)*t;
        if(steep){
            image.set(x, y, color);
        }
        else{
            image.set(y, x, color);
        }
    }
     */
    // 4eme il reset une division a opti
    /*
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    float derror = std::abs(dy/float(dx));
    float error = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error += derror;
        if (error>.5) {
            y += (y1>y0?1:-1);
            error -= 1.;
        }
    }
     */
    // 5eme et dernier
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror3 = 2 * std::abs(dy);
    int error3 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            framebuffer.set(y, x, color);
        } else {
            framebuffer.set(x, y, color);
        }
        error3 += derror3;
        if (error3 > dx) {
            y += dy > 0 ? 1 : -1;
            error3 -= 2 * dx;
        }
    }

}

void Draw::line(Vec<2, int> t0, Vec<2, int> t1, TGAColor color) {
    line(t0[0], t0[1], t1[0], t1[1], color);
}


float surfaceX2(Vec<3, double> &p0, Vec<3, double> &p1, Vec<3, double> &p2) {
    return static_cast<float>((p1.x() - p0.x() )* (p2.y() - p0.y()) - (p2.x() - p0.x()) * (p1.y() - p0.y()));
}
Vec<3, double> barycentric(Vec<3, double> *pts, Vec<3, double> p) {
    float abcAire = surfaceX2(pts[0], pts[1], pts[2]);
    abcAire = abcAire < 1 ? 1 : abcAire;
    float alpha = surfaceX2(p, pts[1], pts[2]) / abcAire;
    float beta  = surfaceX2(p, pts[2], pts[0]) / abcAire;
    float gama  = surfaceX2(p, pts[0], pts[1]) / abcAire;
    return {alpha, beta, gama};
}


void Draw::triangle(Vec<3, double> *pts, TGAColor color) {
    Vec<2, double> boxMin{framebuffer.width()-1.f, framebuffer.height()-1.f};
    Vec<2, double> boxMax{0, 0};
    for (int i = 0; i < 3; ++i) {
        boxMin.x(std::max(0., std::min(boxMin.x(), pts[i].x())));
        boxMin.y(std::max(0., std::min(boxMin.y(), pts[i].y())));
        boxMax.x(std::min(framebuffer.width()-1., std::max(boxMax.x(), pts[i].x())));
        boxMax.y(std::min(framebuffer.width()-1., std::max(boxMax.y(), pts[i].y())));
    }
    for (int x = (int)boxMin.x(); x <= (int)boxMax.x(); x++) {
        for (int y = (int)boxMin.y(); y <= (int)boxMax.y(); y++) {
            Vec<3, double> baryVal = barycentric(pts, {x+0.f, y+0.f, 0.f});
            if(baryVal.x()>=0 && baryVal.y()>=0 && baryVal.z()>=0){
                framebuffer.set(x, y, color);
            }
        }
    }
}

void Draw::trianglezbuff(Vec<3, double> *pts, Model &model, int faceIdx, float intensity, bool texturized) {
    std::vector<Vec<2, double>> texture = model.getTexturePoints(faceIdx);

    Vec<2, double> bboxmin{ std::numeric_limits<float>::max(),  std::numeric_limits<float>::max()};
    Vec<2, double> bboxmax{-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
    for (int i = 0; i < 3; ++i) {
        bboxmin.x(std::max(0., std::min(bboxmin.x(), pts[i].x())));
        bboxmin.y(std::max(0., std::min(bboxmin.y(), pts[i].y())));
        bboxmax.x(std::min(framebuffer.width()-1., std::max(bboxmax.x(), pts[i].x())));
        bboxmax.y(std::min(framebuffer.height()-1., std::max(bboxmax.y(), pts[i].y())));
    }
    TGAColor color;
    color = TGAColor(intensity * 255, intensity * 255, intensity * 255, 255);
    for (int x = (int)bboxmin.x(); x<=(int)bboxmax.x(); x++) {
        for (int y = (int)bboxmin.y(); y<=(int)bboxmax.y(); y++) {
            Vec<3, double> bc_screen  = barycentric(pts, {(float)x, (float)y, 0.f});
            if (bc_screen.x()<0 || bc_screen.y()<0 || bc_screen.z()<0) continue;
            float depth = 0;
            for (int i=0; i<3; i++) depth += pts[i][2]*bc_screen[i];
            if (zbuffer[int(x+y*framebuffer.width())]<depth) {
                zbuffer[int(x+y*framebuffer.width())] = depth;
                if(texturized){
                    //interpolation sur la texture 2d du model
                    double u = bc_screen.x() * texture.at(0).x() + bc_screen.y() * texture.at(1).x() + bc_screen.z() * texture.at(2).x();
                    double v = bc_screen.x() * texture.at(0).y() + bc_screen.y() * texture.at(1).y() + bc_screen.z() * texture.at(2).y();
                    TGAColor c = model.getDiffuse({u, v});
                    color = TGAColor{static_cast<uint8_t>(intensity * c[2]),
                                     static_cast<uint8_t>(intensity * c[1]),
                                     static_cast<uint8_t>(intensity * c[0]),
                                     c[3]
                    };
                }
                framebuffer.set(x, y, color);
            }
        }
    }
}

void Draw::wireframe(Model &model, TGAColor &color){
    for(int i=0; i<model.nbFaces(); i++){
        std::vector<Vec<3, double>> face = model.getFacePoints(i);
        for (int j = 0; j < 3; ++j) {
            auto p0 = face.at(j);
            auto p1 = face.at((j+1)%3);
            int x0 = (p0[0] + 1.) * ((float)framebuffer.width())/2;
            int y0 = (p0[1] + 1.) * ((float)framebuffer.height())/2;
            int x1 = (p1[0] + 1.) * ((float)framebuffer.width())/2;
            int y1 = (p1[1] + 1.) * ((float)framebuffer.height())/2;
            line(x0, y0, x1, y1, color);
        }
    }
}

void Draw::flatshadingrainbow(Model &model){
    for (int i = 0; i < model.nbFaces(); ++i) {
        std::vector<Vec<3, double>> face = model.getFacePoints(i);
        Vec<3, double> screen_coords[3];
        for (int j = 0; j < 3; ++j) {
            screen_coords[j] = {(face.at(j)[0]+1.f)*framebuffer.width()/2.f, (face.at(j)[1]+1.f)*framebuffer.height()/2.f, 0.f};
        }
        triangle(screen_coords, TGAColor(rand()%255, rand()%255, rand()%255, 255));
    }
}

void Draw::backfaceculling(Model &model){

    for (int i = 0; i < model.nbFaces(); ++i) {
        std::vector<Vec<3, double>> face = model.getFacePoints(i);
        Vec<3, double> screen_coords[3];
        for (int j = 0; j < 3; ++j) {
            screen_coords[j] = {(face.at(j)[0]+1)*framebuffer.width()/2, (face.at(j)[1]+1)*framebuffer.height()/2, 0.f};
        }
        Vec<3, double> n = (face.at(2)-face.at(0))^(face.at(1)-face.at(0));
        float intensity = n.normalize() * light_dir;
        if(intensity>0){
            triangle(screen_coords, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255)); //backface culling
        }
    }
}

void Draw::zbufferized(Model &model, bool texturize){
    for (int i = framebuffer.width()*framebuffer.height(); i--; zbuffer[i] = -std::numeric_limits<float>::max());

    for (int i = 0; i < model.nbFaces(); ++i) {
        std::vector<Vec<3, double>> face = model.getFacePoints(i);
        Vec<3, double> screen_coords[3];
        for (int j = 0; j < 3; ++j) {
            screen_coords[j] = getScreenCoords(face[j]);
        }
        Vec<3, double> n = (face.at(2)-face.at(0))^(face.at(1)-face.at(0));
        float intensity = n.normalize() * light_dir.normalize();
        if(intensity>0){
            trianglezbuff(screen_coords, model, i, intensity, texturize);
        }
    }
}

void Draw::lookat(Vec<3, double> eye, Vec<3, double> center, Vec<3, double> up) {
    Vec<3, double> z = (eye-center).normalize();
    Vec<3, double> x = (up^z).normalize();
    Vec<3, double> y = (z^x).normalize();
    Matrix Minv = Matrix<4,4>::identity();
    Matrix Tr   = Matrix<4,4>::identity();
    for (int i=0; i<3; i++) {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];
        Tr[i][3] = -eye[i];
    }
    modelView = Minv*Tr;
}

void Draw::viewport(int x, int y, int w, int h) {
    Matrix m = Matrix<4,4>::identity();
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = DEPTH/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = DEPTH/2.f;
    viewPort = m;
}

void Draw::makeprojection(Vec<3, double> eye, Vec<3, double> center) {
    Matrix m = Matrix<4,4>::identity();
    m[3][2] = -1.f/(eye-center).norm();
    projection = m;
}

void Draw::debug(){
    Vec<3, double> O = getScreenCoords({0,0,0});
    Vec<3, double> x = getScreenCoords({1,0,0});
    Vec<3, double> y = getScreenCoords({0,1,0});
    Vec<3, double> z = getScreenCoords({0,0,1});
    line({static_cast<int>(O.x()), static_cast<int>(O.y())}, {static_cast<int>(x.x()), static_cast<int>(x.y())}, TGAColor{255, 0, 0, 255});
    line({static_cast<int>(O.x()), static_cast<int>(O.y())}, {static_cast<int>(y.x()), static_cast<int>(y.y())}, TGAColor{0, 255, 0, 255});
    line({static_cast<int>(O.x()), static_cast<int>(O.y())}, {static_cast<int>(z.x()), static_cast<int>(z.y())}, TGAColor{0, 0, 255, 255});
}

Vec<3, double> Draw::getScreenCoords(Vec<3, double> coords) {
    Matrix<4,1> mCoords{coords};
    Matrix result = viewPort*projection*modelView*mCoords;
    Vec<3, double> vec(result[0][0]/result[3][0], result[1][0]/result[3][0], result[2][0]/result[3][0]);
    return vec;
}
