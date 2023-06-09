//
// Created by taihangindra on 2023/6/9.
//
#include "../include/tgaimage.h"

bool read_tga_file(char *filename) {
    FILE *fp = fopen(filename, "rb+");
    if(fp == NULL) {
        printf("can't open file: %s", filename);
        return false;
    }
    TGAHeader header;
    if(fread(&header, sizeof(TGAHeader), 1, fp) == 0) {
        printf("an error occured while reading the header");
        return false;
    }


}

TGAColor get_tgacolor(TGAImage *tgaImage, int x, int y) {
    TGAColor ret = {0, 0, 0, 0, 0};
    if (tgaImage == NULL || tgaImage -> data == NULL || x < 0 || y < 0 || x >= tgaImage -> width || y >= tgaImage -> height)
        return ret;
    ret.bpp = tgaImage -> bpp;
    uint8_t *p = tgaImage -> data + (x + y * tgaImage -> width) * tgaImage -> bpp;
    for (int i = tgaImage -> bpp; i--; ret.bgra[i] = p[i]);
    return ret;
}

void set_tgacolor(TGAImage *tgaImage, int x, int y, TGAColor c) {
    if (tgaImage == NULL || tgaImage -> data == NULL || x < 0 || y < 0 || x >= tgaImage -> width || y >= tgaImage -> height)
        return;
    memcpy(tgaImage -> data + (x + y * tgaImage -> width) * tgaImage -> bpp, c.bgra, tgaImage -> bpp);
}

void flip_horizontally(TGAImage *tgaImage) {
    if(tgaImage == NULL) return;
    int w = tgaImage -> width, h = tgaImage -> height, bpp = tgaImage -> bpp;
    int half = w >> 1, tmp;
    uint8_t *data = tgaImage -> data;
    for (int i = 0; i < half; i++)
        for (int j = 0; j < h; j++)
            for (int b = 0; b < bpp; b++) {
                tmp = data[( i+ j * w) * bpp + b];
                data[(i + j * w) * bpp + b] = data[(w - 1 - i + j * w) * bpp + b];
                data[(w - 1 - i + j * w) * bpp + b] = tmp;
            }
}

void flip_vertically(TGAImage *tgaImage) {
    if(tgaImage == NULL) return;
    int w = tgaImage -> width, h = tgaImage -> height, bpp = tgaImage -> bpp;
    int half = w >> 1, tmp;
    uint8_t *data = tgaImage -> data;
    for (int i = 0; i < half; i++)
        for (int j = 0; j < h; j++)
            for (int b = 0; b < bpp; b++) {
                tmp = data[(i + j * w) * bpp + b];
                data[(i + j * w) * bpp + b] = data[(i + (h - 1 - j) * w) * bpp + b];
                data[(i + (h - 1 - j) * w) * bpp + b] = tmp;
            }
}