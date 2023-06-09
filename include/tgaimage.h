//
// Created by taihangindra on 2023/6/9.
//

#ifndef PICORENDERER_TGAIMAGE_H
#define PICORENDERER_TGAIMAGE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

enum Format {
    GRAYSCALE = 1,
    RGB = 3,
    RGBA = 4
};
typedef struct TGAHeader TGAHeader;
typedef struct TGAColor TGAColor;
typedef struct TGAImage TGAImage;

#pragma pack(1)
struct TGAHeader {
    uint8_t  idlength;
    uint8_t  colormaptype;
    uint8_t  datatypecode;
    uint16_t colormaporigin;
    uint16_t colormaplength;
    uint8_t  colormapdepth;
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t  bitsperpixel;
    uint8_t  imagedescriptor;
};
#pragma pack()

struct TGAColor {
    uint8_t bgra[4];
    uint8_t bpp;
};

struct TGAImage {
    int width, height;
    uint8_t bpp;    //bits per pixel, 像素深度（存储每个像素所用的位数）
    uint8_t *data;
};


#endif //PICORENDERER_TGAIMAGE_H
