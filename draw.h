//
// Created by fxzoroark on 12/01/23.
//

#ifndef M3D_PROJECT_M1_DRAW_H
#define M3D_PROJECT_M1_DRAW_H


#include "vec.h"
#include "tgaimage.h"
#include "model.h"
#include "matrix.h"

class Draw {
    TGAImage &framebuffer;
    Vec<3,double> light_dir{0, 0, -1};
    Vec<3, double> eye{1, 1, 3};
    Vec<3, double> center{0, 0, 0};

    float *zbuffer;
    Matrix<4,4> modelView;
    Matrix<4,4> viewPort;
    Matrix<4,4> projection;
    const int DEPTH = 255;

public:
    explicit Draw(TGAImage &framebuffer): framebuffer(framebuffer) {
        zbuffer = new float[framebuffer.width()*framebuffer.height()];
        lookat(eye, center, Vec<3, double>{0,1,0});
        viewport(framebuffer.width()/8, framebuffer.height()/8, framebuffer.width()*3/4, framebuffer.height()*3/4);
        makeprojection(eye, center);
    }

    void line(int x0, int y0, int x1, int y1, TGAColor color);

    void line(Vec<2, int> t0, Vec<2, int> t1, TGAColor color);

    void trianglezbuff(Vec<3, double> *pts, Model &model, int faceIdx, float intensity, bool texturized);

    void triangle(Vec<3, double> *pts, TGAColor color);

    void wireframe(Model &model, TGAColor &color);

    void flatshadingrainbow(Model &model);

    void backfaceculling(Model &model);

    void zbufferized(Model &model, bool texturize = false);
    void lookat(Vec<3, double> eye, Vec<3, double> center, Vec<3, double> up);
    void makeprojection(Vec<3, double> eye, Vec<3, double> center);
    void viewport(int x, int y, int w, int h);

    void debug();
    Vec<3, double> getScreenCoords(Vec<3, double> coords);

};


#endif //M3D_PROJECT_M1_DRAW_H
