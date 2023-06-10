#include <stdio.h>
#include "include/tgaimage.h"

const TGAColor blue   = {255, 0,   0,   255, 3};
const TGAColor white   = {255, 255,   255,   255, 3};

int main() {

//    TGAImage *image = tgaimage_init(100, 100, RGB);
//    tgaimage_set_tgacolor(image, 52, 41, blue);
//    tgaimage_write_tga_file(image, "output.tga", true, true);

    TGAImage *image = tgaimage_read_tga_file("output.tga");
    tgaimage_write_tga_file(image, "output2.tga", true, true);
    return 0;
}
