#include "tgaimage.h"
#include "model.h"
#include "draw.h"
#include <vector>
#include <tuple>
#include <iostream>

static constexpr int width = 1024;
static constexpr int height = 1024;
static TGAColor white = {255, 255, 255, 255};
static TGAColor green = {0, 255, 0, 255};
static TGAColor black = {0, 0, 0, 255};

void clearbuffer(TGAImage &buffer){
    for (int x = 0; x < buffer.width(); ++x) {
        for (int y = 0; y < buffer.height(); ++y) {
            buffer.set(x, y, black);
        }
    }
}

int main() {

    TGAImage framebuffer{width, height, TGAImage::RGB};
    Draw draw{framebuffer};
    Model head_model{"../obj/african_head/african_head"};
    Model diablo_model{"../obj/diablo3_pose/diablo3_pose"};
    draw.debug();
    /*

    //Lesson 1
    draw.line(13, 20, 80, 40, framebuffer, white);
    draw.line(20, 13, 40, 80, framebuffer, green);
    draw.line(80, 40, 13, 20, framebuffer, green);
    framebuffer.write_tga_file("../rendu/Bresenhams_Line.tga");
    clearbuffer(framebuffer);


    draw.wireframe(head_model, white);
    framebuffer.write_tga_file("../rendu/wireframe.tga");
    clearbuffer(framebuffer);

    //Lesson 2
    draw.flatshadingrainbow(head_model);
    framebuffer.write_tga_file("../rendu/flatshading_rainbow.tga");
    clearbuffer(framebuffer);
    draw.backfaceculling(head_model);
    framebuffer.write_tga_file("../rendu/backface_culling.tga");
    clearbuffer(framebuffer);



    //Lesson 3
    draw.zbufferized(head_model);
    framebuffer.write_tga_file("../rendu/zbufferized.tga");
    clearbuffer(framebuffer);

    draw.zbufferized(head_model, true);
    framebuffer.write_tga_file("../rendu/zbufferized_textured.tga");
    clearbuffer(framebuffer);


    //Lesson 4 Marche avec l'ajout de la camera (Lesson 5)
    //draw.zbufferized(head_model, true, true);
    //framebuffer.write_tga_file("../rendu/zbufferized_perspectives.tga");
    //clearbuffer(framebuffer);

     */

    //Lesson 5

    draw.zbufferized(head_model, true);
    framebuffer.write_tga_file("../rendu/camera.tga");
    clearbuffer(framebuffer);
    return 0;
}
