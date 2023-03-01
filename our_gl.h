//
// Created by fxzoroark on 03/02/23.
//

#ifndef M3D_PROJECT_M1_OUR_GL_H
#define M3D_PROJECT_M1_OUR_GL_H


#include "tgaimage.h"
#include "matrix.h"


extern Matrix<4,4> modelView;
extern Matrix<4,4> viewport;
extern Matrix<4,4> projection;

const int DEPTH = 255;


void setViewport(int x, int y, int w, int h);
void setProjection(Vec<3, double> eye, Vec<3, double> center);
void lookat(Vec<3, double> eye, Vec<3, double> center, Vec<3, double> up);

struct IShader {
    virtual ~IShader();
    virtual Vec<4, double> vertex(int iface, int nthvert) = 0;
    virtual bool fragment(Vec<3, double> bar, TGAColor &color) = 0;
};

void triangle(Vec<4, double> *pts, IShader &shader, TGAImage &image, float *zbuffer);

#endif //M3D_PROJECT_M1_OUR_GL_H