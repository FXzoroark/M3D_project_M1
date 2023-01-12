#include "tgaimage.h"
#include "model.h"
#include <vector>
#include <tuple>
#include <iostream>

static constexpr int width = 1024;
static constexpr int height = 1024;



void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
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
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error3 += derror3;
        if (error3 > dx) {
            y += dy > 0 ? 1 : -1;
            error3 -= 2 * dx;
        }
    }

}

void line(Vec2<int> t0, Vec2<int> t1, TGAImage &image, TGAColor color) {
    line(t0[0], t0[1], t1[0], t1[1], image, color);
}

float surfaceX2(Vec3<float> &p0, Vec3<float> &p1, Vec3<float> &p2) {
    return static_cast<float>((p1.x - p0.x )* (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y));
}
Vec3<float> barycentric(Vec3<float> *pts, Vec3<float> p) {
    float abcAire = surfaceX2(pts[0], pts[1], pts[2]);
    abcAire = abcAire < 1 ? 1 : abcAire;
    float alpha = surfaceX2(p, pts[1], pts[2]) / abcAire;
    float beta  = surfaceX2(p, pts[2], pts[0]) / abcAire;
    float gama  = surfaceX2(p, pts[0], pts[1]) / abcAire;
    return {alpha, beta, gama};
}

void triangle(Vec3<float> *pts, TGAImage &image, TGAColor color) {
    Vec2<float> boxMin{width-1, height-1};
    Vec2<float> boxMax{0, 0};
    for (int i = 0; i < 3; ++i) {
        boxMin.x = std::max(0.f, std::min(boxMin.x, pts[i].x));
        boxMin.y = std::max(0.f, std::min(boxMin.y, pts[i].y));
        boxMax.x = std::min(width-1.f, std::max(boxMax.x, pts[i].x));
        boxMax.y = std::min(height-1.f, std::max(boxMax.y, pts[i].y));
    }
    for (int x = (int)boxMin.x; x <= (int)boxMax.x; x++) {
        for (int y = (int)boxMin.y; y <= (int)boxMax.y; y++) {
            Vec3<float> baryVal = barycentric(pts, {x+0.f, y+0.f, 0});
            if(baryVal.x>=0 && baryVal.y>=0 && baryVal.z>=0){
                image.set(x, y, color);
            }
        }
    }
}

void triangle(Vec3<float> *pts, float *zbuffer, TGAImage &image, TGAColor color) {
    Vec2<float> bboxmin{ std::numeric_limits<float>::max(),  std::numeric_limits<float>::max()};
    Vec2<float> bboxmax{-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
    for (int i = 0; i < 3; ++i) {
        bboxmin.x = std::max(0.f, std::min(bboxmin.x, pts[i].x));
        bboxmin.y = std::max(0.f, std::min(bboxmin.y, pts[i].y));
        bboxmax.x = std::min(width-1.f, std::max(bboxmax.x, pts[i].x));
        bboxmax.y = std::min(height-1.f, std::max(bboxmax.y, pts[i].y));
    }

    for (int x = (int)bboxmin.x; x<=(int)bboxmax.x; x++) {
        for (int y = (int)bboxmin.y; y<=(int)bboxmax.y; y++) {
            Vec3<float> bc_screen  = barycentric(pts, {(float)x, (float)y, 0.f});
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
            float depth = 0;
            for (int i=0; i<3; i++) depth += pts[i][2]*bc_screen[i];
            if (zbuffer[int(x+y*width)]<depth) {
                zbuffer[int(x+y*width)] = depth;
                image.set(x, y, color);
            }
        }
    }
}



int main() {

    const TGAColor white = {255, 255, 255, 255};
    const TGAColor green = {0, 255, 0, 255};

    TGAImage framebuffer(width, height, TGAImage::RGB);
    //line(13, 20, 80, 40, framebuffer, white);
    //line(20, 13, 40, 80, framebuffer, green);
    //line(80, 40, 13, 20, framebuffer, green);
    Model model{"../obj/african_head/african_head.obj"};
//    for(int i=0; i<model.nbFaces(); i++){
//        std::vector<Vec3<float>> face = model.getFacePoints(i);
//        for (int j = 0; j < 3; ++j) {
//            auto p0 = face.at(j);
//            auto p1 = face.at((j+1)%3);
//            int x0 = (p0[0] + 1.) * width/2;
//            int y0 = (p0[1] + 1.) * height/2;
//            int x1 = (p1[0] + 1.) * width/2;
//            int y1 = (p1[1] + 1.) * height/2;
//            line(x0, y0, x1, y1, framebuffer, white);
//        }
//    }
//    for (int i = 0; i < model.nbFaces(); ++i) {
//        std::vector<Vec3<float>> face = model.getFacePoints(i);
//        Vec2<int> screen_coords[3];
//        for (int j = 0; j < 3; ++j) {
//            screen_coords[j] = {static_cast<int>((face.at(j)[0]+1.)*width/2.), static_cast<int>((face.at(j)[1]+1.)*height/2.)};
//        }
//        triangle(screen_coords, framebuffer, TGAColor(rand()%255, rand()%255, rand()%255, 255));
//    }
//    Vec3<float> light_dir{0, 0, -1};
//
//    for (int i = 0; i < model.nbFaces(); ++i) {
//        std::vector<Vec3<float>> face = model.getFacePoints(i);
//        Vec2<int> screen_coords[3];
//        for (int j = 0; j < 3; ++j) {
//            screen_coords[j] = {static_cast<int>((face.at(j)[0]+1)*width/2), static_cast<int>((face.at(j)[1]+1)*height/2), face};
//        }
//        Vec3<float> n = (face.at(2)-face.at(0))^(face.at(1)-face.at(0));
//        float intensity = n.normalize() * light_dir;
//        if(intensity>0){
//            triangle(screen_coords, framebuffer, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255)); //backface culling
//        }
//    }

    Vec3<float> light_dir{0, 0, -1};
    auto *zbuffer = new float[width*height];
    for (int i=width*height; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    for (int i = 0; i < model.nbFaces(); ++i) {
        std::vector<Vec3<float>> face = model.getFacePoints(i);
        Vec3<float> screen_coords[3];
        for (int j = 0; j < 3; ++j) {
            screen_coords[j] = {(face.at(j)[0]+1.f)*width/2.f, (face.at(j)[1]+1.f)*height/2.f, face.at(j)[2]};
        }
        Vec3<float> n = (face.at(2)-face.at(0))^(face.at(1)-face.at(0));
        float intensity = n.normalize() * light_dir;
        if(intensity>0){
            triangle(screen_coords, zbuffer, framebuffer, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255)); //zbuffer
            //triangle(screen_coords, framebuffer, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255)); //backtrack culling
        }
    }
    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
