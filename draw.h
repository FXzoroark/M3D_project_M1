//
// Created by fxzoroark on 12/01/23.
//

#ifndef M3D_PROJECT_M1_DRAW_H
#define M3D_PROJECT_M1_DRAW_H


#include <utility>

#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

class Draw {
    TGAImage &framebuffer;
    Vec3<float> light_dir{0, 0, -1};
    Vec3<float> eye{1, 1, 3};
    Vec3<float> center{0, 0, 0};

    float *zbuffer;
    Matrix modelView;
    Matrix viewPort;
    Matrix projection;
    const int DEPTH = 255;

public:
    explicit Draw(TGAImage &framebuffer): framebuffer(framebuffer) {
        zbuffer = new float[framebuffer.width()*framebuffer.height()];
        lookat(eye, center, Vec3<float>{0,1,0});
        viewport(framebuffer.width()/8, framebuffer.height()/8, framebuffer.width()*3/4, framebuffer.height()*3/4);
        makeprojection(eye, center);
    }

    void line(int x0, int y0, int x1, int y1, TGAColor color);

    void line(Vec2<int> t0, Vec2<int> t1, TGAColor color);

    void trianglezbuff(Vec3<float> *pts, Model &model, int faceIdx, float intensity, bool texturized);

    void triangle(Vec3<float> *pts, TGAColor color);

    void wireframe(Model &model, TGAColor &color);

    void flatshadingrainbow(Model &model);

    void backfaceculling(Model &model);

    void zbufferized(Model &model, bool texturize = false);
    void lookat(Vec3<float> eye, Vec3<float> center, Vec3<float> up);
    void makeprojection(Vec3<float> eye, Vec3<float> center);
    void viewport(int x, int y, int w, int h);

    void debug();
    Vec3<float> getScreenCoords(Vec3<float> coords);

};


#endif //M3D_PROJECT_M1_DRAW_H
