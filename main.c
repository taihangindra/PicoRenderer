#include <math.h>
#include "include/tgaimage.h"
#include "include/geometry.h"

const TGAColor white = {255, 255,   255,   255, 3};
const TGAColor blue = {255, 0,   0,   255, 3};
const TGAColor green = {0, 255, 0, 255, 3};
const TGAColor red = {0, 0, 255, 255, 3};
const int width  = 800;
const int height = 500;

void line(TGAImage *image, Vec2i t0, Vec2i t1, TGAColor color) {
    int x0 = t0.x, y0 = t0.y, x1 = t1.x, y1 = t1.y;
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

Vec3f barycentric(Vec2i *pts, Vec2i P) {
    Vec3f a = {pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x},
          b = {pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y};
    Vec3f u = vec3f_cross(a, b);
    /* `pts` and `P` has integer value as coordinates
       so `abs(u[2])` < 1 means `u[2]` is 0, that means
       triangle is degenerate, in this case return something with negative coordinates */
    Vec3f res;
    if (abs(u.z)<1) {
        res.x = -1, res.y = 1, res.z = 1;
    }
    else {
        res.x = 1.f - (u.x + u.y) / u.z,
        res.y = u.y / u.z,
        res.z = u.x / u.z;
    }
    return res;
}

void triangle(TGAImage* image, Vec2i *pts, TGAColor color) {
    if(image) {
        Vec2i bboxmin = {image -> width - 1, image -> height - 1};
        Vec2i bboxmax = {0, 0};
        Vec2i clamp = {image -> width - 1, image -> height - 1};
        for (int i = 0; i < 3; i++) {
            bboxmin.x = fmax(0, fmin(bboxmin.x, pts[i].x));
            bboxmin.y = fmax(0, fmin(bboxmin.y, pts[i].y));

            bboxmax.x = fmin(clamp.x, fmax(bboxmax.x, pts[i].x));
            bboxmax.y = fmin(clamp.y, fmax(bboxmax.y, pts[i].y));
        }
        Vec2i P;
        for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
            for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
                Vec3f bc_screen  = barycentric(pts, P);
                if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
                tgaimage_set_tgacolor(image, P.x, P.y, color);
            }
        }
    }
}

void rasterize(Vec2i p0, Vec2i p1, TGAImage *image, TGAColor color, int ybuffer[]) {
    if (p0.x>p1.x) {
        swap(p0, p1, Vec2i);
    }
    for (int x=p0.x; x<=p1.x; x++) {
        float t = (x-p0.x)/(float)(p1.x-p0.x);
        int y = p0.y*(1.-t) + p1.y*t;
        if (ybuffer[x]<y) {
            ybuffer[x] = y;
            tgaimage_set_tgacolor(image, x, 0, color);
        }
    }
}

int main(int argc, char **argv) {
    Vec2i pts[4][2] = {{{20, 34}, {744, 400}}, {{120, 434}, {444, 400}},
                       {{330, 463}, {594, 200}}, {{10, 10}, {790, 10}}};
    { // just dumping the 2d scene (yay we have enough dimensions!)
        TGAImage *scene = tgaimage_init(width, height, RGB);

        // scene "2d mesh"
        line(scene, pts[0][0], pts[0][1], red);
        line(scene, pts[1][0], pts[1][1], green);
        line(scene, pts[2][0], pts[2][1], blue);

        // screen line
        line(scene, pts[3][0], pts[3][1],white);

        tgaimage_write_tga_file(scene, "scene.tga", true, true);
    }

    {
        TGAImage *render = tgaimage_init(width, 16, RGB);
        int ybuffer[width];
        for (int i = 0; i < width; i++) {
            ybuffer[i] = INT_MIN;
        }
        rasterize(pts[0][0], pts[0][1], render, red,   ybuffer);
        rasterize(pts[1][0], pts[1][1], render, green, ybuffer);
        rasterize(pts[2][0], pts[2][1], render, blue,  ybuffer);

        // 1-pixel wide image is bad for eyes, lets widen it
        for (int i = 0; i < width; i++) {
            for (int j = 1; j < 16; j++) {
                tgaimage_set_tgacolor(render, i, j, tgaimage_get_tgacolor(render, i, 0));
            }
        }
        tgaimage_write_tga_file(render, "render.tga", true, true);
    }
    return 0;
}
