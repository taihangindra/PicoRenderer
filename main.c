#include <math.h>
#include "include/tgaimage.h"

const TGAColor blue = {255, 0,   0,   255, 3};
const TGAColor red = {0, 0, 255, 255, 3};
const TGAColor white = {255, 255,   255,   255, 3};

void line(TGAImage *image, int x0, int y0, int x1, int y1, TGAColor color) {
    if (image) {
        bool steep = false;
        if (abs(x0 - x1) < abs(y0 - y1)) {
            swap(x0, y0, int);
            swap(x1, y1, int);
            steep = true;
        }
        if (x0 > x1) {
            swap(x0, x1, int);
            swap(y0, y1, int);
        }
        int dx = x1 - x0;
        int dy = y1 - y0;
        int derror = abs(dy) * 2;
        int error = 0;
        int y = y0;
        for (int x = x0; x <= x1; x++) {
            if (steep) {
                tgaimage_set_tgacolor(image, y, x, color);
            } else {
                tgaimage_set_tgacolor(image, x, y, color);
            }
            error += derror;
            if (error > dx) {
                y += (y1 > y0 ? 1 : -1);
                error -= dx * 2;
            }
        }
    }
}

int main() {
    TGAImage *image = tgaimage_init(100, 100, RGB);

    line(image, 13, 20, 80, 40, white);
    line(image, 20, 13, 40, 80, white);
    line(image, 80, 40, 13, 20, red);

    tgaimage_write_tga_file(image, "output.tga", true, true);
    return 0;
}
