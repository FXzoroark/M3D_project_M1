#include "tgaimage.h"
#include "model.h"
#include "draw.h"
#include "our_gl.h"
#include <vector>
#include <tuple>
#include <iostream>

static constexpr int width = 2048; //8192 * 8192
static constexpr int height = 2048;
static TGAColor white = {255, 255, 255, 255};
static TGAColor green = {0, 255, 0, 255};
static TGAColor black = {0, 0, 0, 255};

//post shaders refactor (Lesson 6)
Model *model     = NULL;

Vec<3, double> light_dir(1,1,0.5); //1, 1, 0.5
Vec<3, double>       eye(1,1,3);
Vec<3, double>    center(0,0,0);
Vec<3, double>        up(0,1,0);

void clearbuffer(TGAImage &buffer){
    for (int x = 0; x < buffer.width(); ++x) {
        for (int y = 0; y < buffer.height(); ++y) {
            buffer.set(x, y, black);
        }
    }
}


struct GouraudShader : public IShader {
    Vec<3, double> varying_intensity; // written by vertex shader, read by fragment shader

    Vec<4, double> vertex(int iface, int nthvert) {
        varying_intensity[nthvert] = std::max(0., model->getFaceNormals(iface).at(nthvert)*light_dir); // get diffuse lighting intensity
        Matrix<4,1> gl_Vertex = model->getFacePoints(iface).at(nthvert); // read the vertex from .obj file
        gl_Vertex = viewport*projection*modelView*gl_Vertex;
        return {gl_Vertex[0][0]/gl_Vertex[3][0], gl_Vertex[1][0]/gl_Vertex[3][0], gl_Vertex[2][0]/gl_Vertex[3][0], gl_Vertex[3][0]/gl_Vertex[3][0]}; // transform it to screen coordinates
    }

    bool fragment(Vec<3, double> bar, TGAColor &color) {
       // std::cout << varying_intensity.x() << " " << varying_intensity.y() << " " << varying_intensity.z() << std::endl;
        //std::cout << bar.x() << " " << bar.y() << " " << bar.z() << std::endl;

        double intensity = varying_intensity*bar;   // interpolate intensity for the current pixel
        color = TGAColor(255, 255, 255)*intensity; // well duh
        return false;                              // no, we do not discard this pixel
    }
};

struct GouraudEditShader : public IShader{
    Vec<3, double> varying_intensity;

    Vec<4, double> vertex(int iface, int nthvert) {
        varying_intensity[nthvert] = std::max(0., model->getFaceNormals(iface).at(nthvert)*light_dir); // get diffuse lighting intensity
        Matrix<4,1> gl_Vertex = model->getFacePoints(iface).at(nthvert); // read the vertex from .obj file
        gl_Vertex = viewport*projection*modelView*gl_Vertex;
        return {gl_Vertex[0][0]/gl_Vertex[3][0], gl_Vertex[1][0]/gl_Vertex[3][0], gl_Vertex[2][0]/gl_Vertex[3][0], gl_Vertex[3][0]/gl_Vertex[3][0]}; // transform it to screen coordinates
    }

    bool fragment(Vec<3, double> bar, TGAColor &color) {
        float intensity = varying_intensity*bar;
        if (intensity>.85) intensity = 1;
        else if (intensity>.60) intensity = .80;
        else if (intensity>.45) intensity = .60;
        else if (intensity>.30) intensity = .45;
        else if (intensity>.15) intensity = .30;
        else intensity = 0;
        color = TGAColor(255, 155, 0) * intensity;
        return false;
    }

};

struct TexturedShader : IShader{
    Vec<3, double> varying_intensity; // written by vertex shader, read by fragment shader
    Matrix<2,3>    varying_uv;        // same as above

    Vec<4, double> vertex(int iface, int nthvert) {
        varying_uv.set_col(nthvert, model->getTexturePoints(iface).at(nthvert));
        varying_intensity[nthvert] = std::max(0., model->getFaceNormals(iface).at(nthvert)*light_dir); // get diffuse lighting intensity
        Matrix<4,1> gl_Vertex = model->getFacePoints(iface).at(nthvert); // read the vertex from .obj file
        gl_Vertex = viewport*projection*modelView*gl_Vertex;
        return {gl_Vertex[0][0]/gl_Vertex[3][0], gl_Vertex[1][0]/gl_Vertex[3][0], gl_Vertex[2][0]/gl_Vertex[3][0], gl_Vertex[3][0]/gl_Vertex[3][0]}; // transform it to screen coordinates
    }

    bool fragment(Vec<3, double> bar, TGAColor &color) {
        float intensity = varying_intensity*bar;   // interpolate intensity for the current pixel
        Vec<2, double> uv = varying_uv*bar;                 // interpolate uv for the current pixel
        color = model->getDiffuse(uv)*intensity;      // well duh
        return false;                              // no, we do not discard this pixel
    }
};

struct Shader : public IShader {
    Matrix<2,3> varying_uv;  // same as above
    Matrix<4,4> uniform_M;   //  Projection*ModelView
    Matrix<4,4> uniform_MIT; // (Projection*ModelView).invert_transpose()

    Vec<4, double> vertex(int iface, int nthvert) {
        varying_uv.set_col(nthvert, model->getTexturePoints(iface).at(nthvert));
        Matrix<4,1> gl_Vertex = model->getFacePoints(iface).at(nthvert); // read the vertex from .obj file
        gl_Vertex = viewport*projection*modelView*gl_Vertex;
        return {gl_Vertex[0][0]/gl_Vertex[3][0], gl_Vertex[1][0]/gl_Vertex[3][0], gl_Vertex[2][0]/gl_Vertex[3][0], gl_Vertex[3][0]/gl_Vertex[3][0]}; // transform it to screen coordinates

    }

    bool fragment(Vec<3, double> bar, TGAColor &color) override {
        Vec<2, double> uv = varying_uv*bar;                 // interpolate uv for the current pixel
        Vec<3, double> n = (uniform_MIT*model->getNormal(uv).proj<4>()).normalize();
        Vec<3, double> l = (uniform_M*light_dir.proj<4>()).normalize();
        Vec<3, double> r = (n.scl(n*l*2.) - l).normalize();   // reflected light
        //double spec = pow(std::max(r*eye.normalize(), 0.), model->getSpecular(uv) > 0 ? model->getSpecular(uv) : 1);
        double spec = pow(std::max(r.z(), 0.), 1+model->getSpecular(uv));
        double diff = std::max(0., n*l);
        TGAColor c = model->getDiffuse(uv);
        for (int i=0; i<3; i++) color[i] = std::min<double>(20 + c[i]*(1.2*diff + 0.8*spec), 255);
        return false;                              // no, we do not discard this pixel
    }
};

void shadering(IShader &shader, TGAImage &framebuffer){
    auto *zbuffer = new float[framebuffer.width()*framebuffer.height()];
    for (int i = framebuffer.width()*framebuffer.height(); i--; zbuffer[i] = -std::numeric_limits<float>::max());

    for (int i=0; i<model->nbFaces(); i++) {
        Vec<4, double> screen_coords[3];
        for (int j=0; j<3; j++) {
            screen_coords[j] = shader.vertex(i, j);
        }
        triangle(screen_coords, shader, framebuffer, zbuffer);
    }
}

int main() {

    TGAImage framebuffer{width, height, TGAImage::RGB};
    Draw draw{framebuffer};
    model = new Model{"../obj/african_head/african_head"};
    //model = new Model{"../obj/diablo3_pose/diablo3_pose"};
    //draw.debug();


    //Lesson 1
//    draw.line(13, 20, 80, 40, white);
//    draw.line(20, 13, 40, 80, green);
//    draw.line(80, 40, 13, 20, green);
//    framebuffer.write_tga_file("../rendu/Bresenhams_Line.tga");
//    clearbuffer(framebuffer);
//
//
//    draw.wireframe(head_model, white);
//    framebuffer.write_tga_file("../rendu/wireframe.tga");
//    clearbuffer(framebuffer);
//
//    //Lesson 2
//    draw.flatshadingrainbow(head_model);
//    framebuffer.write_tga_file("../rendu/flatshading_rainbow.tga");
//    clearbuffer(framebuffer);
//    draw.backfaceculling(head_model);
//    framebuffer.write_tga_file("../rendu/backface_culling.tga");
//    clearbuffer(framebuffer);
//
//
//
//    //Lesson 3
//    draw.zbufferized(head_model);
//    framebuffer.write_tga_file("../rendu/zbufferized.tga");
//    clearbuffer(framebuffer);
//
//    //Marche avec l'ajout de la camera
//    draw.zbufferized(head_model, true);
//    framebuffer.write_tga_file("../rendu/zbufferized_textured.tga");
//    clearbuffer(framebuffer);


    //Lesson 4 Marche avec l'ajout de la camera (Lesson 5)
    //draw.zbufferized(head_model, true, true);
    //framebuffer.write_tga_file("../rendu/zbufferized_perspectives.tga");
    //clearbuffer(framebuffer);



    //Lesson 5

//    draw.zbufferized(model, true);
//    framebuffer.write_tga_file("../rendu/camera.tga");
//    clearbuffer(framebuffer);

    //Lesson 6

    lookat(eye, center, up);
    setViewport(framebuffer.width()/8, framebuffer.height()/8, framebuffer.width()*3/4, framebuffer.height()*3/4);
    setProjection(eye, center);
    light_dir = light_dir.normalize();


//    GouraudShader gouraudShader;
//    shadering(gouraudShader, framebuffer);
//
//    framebuffer.write_tga_file("../rendu/GouraudShader.tga");
//    clearbuffer(framebuffer);
//
//
//    GouraudEditShader gouraudEditShader;
//    shadering(gouraudEditShader, framebuffer);
//
//    framebuffer.write_tga_file("../rendu/GouraudEditShader.tga");
//    clearbuffer(framebuffer);
//
//    TexturedShader texturedShader;
//    shadering(texturedShader, framebuffer);
//
//    framebuffer.write_tga_file("../rendu/TexturedShader.tga");
//    clearbuffer(framebuffer);

    Shader shader;
    shader.uniform_M = projection * modelView;
    shader.uniform_MIT = Matrix<4,4>::invert44((projection*modelView).transpose());

    shadering(shader, framebuffer);

    framebuffer.write_tga_file("../rendu/Shader.tga");
    clearbuffer(framebuffer);





    //delete model;
    return 0;
}
