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
    float *zbuffer;

public:
    explicit Draw(TGAImage &framebuffer): framebuffer(framebuffer) {zbuffer = new float[framebuffer.width()*framebuffer.height()];}

    void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

    void line(Vec2<int> t0, Vec2<int> t1, TGAImage &image, TGAColor color);

    void trianglezbuff(Vec3<float> *pts, Model &model, int faceIdx, float intensity, bool texturized);

    void triangle(Vec3<float> *pts, TGAColor color);

    void wireframe(Model &model, TGAColor &color);

    void flatshadingrainbow(Model &model);

    void backfaceculling(Model &model);

    void zbufferized(Model &model, bool texturize = false);
};


#endif //M3D_PROJECT_M1_DRAW_H
