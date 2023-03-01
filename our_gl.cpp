//
// Created by fxzoroark on 03/02/23.
//

#include "our_gl.h"

Matrix<4,4> modelView;
Matrix<4,4> viewport;
Matrix<4,4> projection;


void lookat(Vec<3, double> eye, Vec<3, double> center, Vec<3, double> up) {
    Vec<3, double> z = (eye-center).normalize();
    Vec<3, double> x = (up^z).normalize();
    Vec<3, double> y = (z^x).normalize();
    Matrix Minv = Matrix<4,4>::identity();
    //Matrix Tr   = Matrix<4,4>::identity();
    for (int i=0; i<3; i++) {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];
        //Tr[i][3] = -eye[i];
        Minv[i][3] = -center[i];

    }
    //modelView = Minv*Tr;
    modelView = Minv;
}

void setViewport(int x, int y, int w, int h) {
    Matrix m = Matrix<4,4>::identity();
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = DEPTH/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = DEPTH/2.f;
    viewport = m;
}

void setProjection(Vec<3, double> eye, Vec<3, double> center) {
    Matrix m = Matrix<4,4>::identity();
    m[3][2] = -1.f/(eye-center).norm();
    projection = m;
}

double surfaceX2(Vec<4, double> &p0, Vec<4, double> &p1, Vec<4, double> &p2) {
    return static_cast<double>((p1[0] - p0[0])* (p2[1] - p0[1]) - (p2[0] - p0[0]) * (p1[1] - p0[1]));

}
Vec<3, double> barycentric(Vec<4, double> *pts, Vec<4, double> p) {
    double abcAire = surfaceX2(pts[0], pts[1], pts[2]);
    abcAire = abcAire < 1e-2 ? 1 : abcAire;
    double alpha = surfaceX2(p, pts[1], pts[2]) / abcAire;
    double beta  = surfaceX2(p, pts[2], pts[0]) / abcAire;
    double gama  = surfaceX2(p, pts[0], pts[1]) / abcAire;
    return {alpha, beta, gama};
}

void triangle(Vec<4, double> *pts, IShader &shader, TGAImage &image, float *zbuffer) {
    Vec<2, double> bboxmin{ std::numeric_limits<float>::max(),  std::numeric_limits<float>::max()};
    Vec<2, double> bboxmax{-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
    for (int i = 0; i < 3; ++i) {
        bboxmin.x(std::max(0., std::min(bboxmin.x(), pts[i][0])));
        bboxmin.y(std::max(0., std::min(bboxmin.y(), pts[i][1])));
        bboxmax.x(std::min(image.width()-1., std::max(bboxmax.x(), pts[i][0])));
        bboxmax.y(std::min(image.height()-1., std::max(bboxmax.y(), pts[i][1])));
    }
    TGAColor color;
    for (int x = (int)bboxmin.x(); x<=(int)bboxmax.x(); x++) {
        for (int y = (int)bboxmin.y(); y<=(int)bboxmax.y(); y++) {
            Vec<3, double> bc_screen  = barycentric(pts, {(double)x, (double)y, 0.f, 0.f});
            if (bc_screen.x()<0 || bc_screen.y()<0 || bc_screen.z()<0) continue;
            float depth = 0;
            for (int i=0; i<3; i++) depth += pts[i][2]*bc_screen[i];

            if (zbuffer[int(x+y*image.width())]<depth) {
                zbuffer[int(x+y*image.width())] = depth;
                if(!shader.fragment(bc_screen, color)){
                    image.set(x, y, color);
                }
            }
        }
    }
}

IShader::~IShader() {}

