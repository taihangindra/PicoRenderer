//
// Created by taihangindra on 2023/6/9.
//
#include "../include/tgaimage.h"

static bool tgaimage_load_rle_data(TGAImage *tgaImage, FILE *fp);
static bool tgaimage_unload_rle_data(TGAImage *tgaImage, FILE *fp);

TGAImage* tgaimage_init(int width, int height, int bpp) {
    TGAImage *ti = malloc(sizeof(TGAImage));
    if(ti == NULL) {
        printf("memory is not enough\n");
        return NULL;
    }

    size_t nBytes = width * height * bpp;
    ti -> width = width;
    ti -> height = height;
    ti -> bpp = bpp;
    ti -> data = malloc(nBytes);
    if(ti -> data == NULL) {
        free(ti);
        printf("memory is not enough\n");
        return NULL;
    }
    memset(ti -> data, 0, nBytes);

    return ti;
}

TGAImage* tgaimage_read_tga_file(char *filename) {
    TGAImage *ti = malloc(sizeof(TGAImage));
    if(ti == NULL) {
        printf("memory is not enough\n");
        return ti;
    }

    FILE *fp = fopen(filename, "rw+");
    if(fp == NULL) {
        printf("can't open file: %s\n", filename);
        goto error1;
    }
    TGAHeader header;
    if(fread(&header, sizeof(TGAHeader), 1, fp) == 0) {
        printf("an error occured while reading the header\n");
        goto error1;
    }

    ti -> width = header.width;
    ti -> height = header.height;
    ti -> bpp = header.bitsperpixel >> 3;
    if(ti -> width <= 0 || ti -> height <= 0 || (ti -> bpp != GRAYSCALE && ti -> bpp != RGB && ti -> bpp != RGBA)) {
        printf("bad bpp (or width/height) value\n");
        goto error1;
    }

    size_t nbytes = ti -> width * ti -> height * ti -> bpp;
    ti -> data = malloc(nbytes);
    if(ti -> data == NULL) {
        printf("memory is not enough\n");
        goto error1;
    }
    memset(ti -> data, 0, nbytes);

    if (3 == header.datatypecode || 2 == header.datatypecode) {
        if(fread(ti -> data, 1, nbytes, fp) == 0) {
            printf("an error occured while reading the data\n");
            goto error2;
        }
    } else if (10 == header.datatypecode || 11 == header.datatypecode) {
        if (!tgaimage_load_rle_data(ti, fp)) {
            printf("an error occured while reading the data\n");
            goto error2;
        }
    } else {
        printf("unknown file format %hhu\n", header.datatypecode);
        goto error2;
    }
    if (!(header.imagedescriptor & 0x20))
        tgaimage_flip_vertically(ti);
    if (header.imagedescriptor & 0x10)
        tgaimage_flip_horizontally(ti);

    success:
    fclose(fp);
    return ti;

    error2:
    free(ti -> data);
    error1:
    free(ti);
    fclose(fp);
    return NULL;
}

bool tgaimage_write_tga_file(TGAImage *tgaImage, char* filename, bool vflip, bool rle) {
    uint8_t developer_area_ref[4] = {0, 0, 0, 0};
    uint8_t extension_area_ref[4] = {0, 0, 0, 0};
    uint8_t footer[18] = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};
    FILE *fp = fopen(filename, "wb+");
    if(fp == NULL) {
        printf("can't open file : %s\n", filename);
        fclose(fp);
        return fp;
    }
    TGAHeader header;
    memset(&header, 0, sizeof(TGAHeader));
    header.bitsperpixel = tgaImage -> bpp << 3;
    header.width  = tgaImage -> width;
    header.height = tgaImage -> height;
    header.datatypecode = ( tgaImage -> bpp == GRAYSCALE ? (rle ? 11 : 3) : (rle ? 10 : 2));
    header.imagedescriptor = vflip ? 0x00 : 0x20; // top-left or bottom-left origin
    if (fwrite(&header, sizeof(header), 1, fp) == 0) {
        printf("can't dump the tga file\n");
        fclose(fp);
        return false;
    }
    if (!rle) {
        if (fwrite(tgaImage -> data, 1, tgaImage -> width * tgaImage -> height * tgaImage -> bpp, fp) == 0) {
            printf("can't unload rle data\n");
            fclose(fp);
            return false;
        }
    } else if (!tgaimage_unload_rle_data(tgaImage, fp)) {
        printf("can't unload rle data\n");
        fclose(fp);
        return false;
    }
    if (fwrite(&developer_area_ref, sizeof(developer_area_ref), 1, fp) == 0) {
        printf("can't dump the tga file\n");
        fclose(fp);
        return false;
    }
    if (fwrite(&developer_area_ref, sizeof(developer_area_ref), 1, fp) == 0) {
        printf("can't dump the tga file\n");
        fclose(fp);
        return false;
    }
    if (fwrite(&footer, sizeof(footer), 1, fp) == 0) {
        printf("can't dump the tga file\n");
        fclose(fp);
        return false;
    }
    fclose(fp);
    return true;
}

TGAColor tgaimage_get_tgacolor(TGAImage *tgaImage, int x, int y) {
    TGAColor ret = {0, 0, 0, 0, 0};
    if (tgaImage == NULL || tgaImage -> data == NULL || x < 0 || y < 0 || x >= tgaImage -> width || y >= tgaImage -> height)
        return ret;
    ret.bpp = tgaImage -> bpp;
    uint8_t *p = tgaImage -> data + (x + y * tgaImage -> width) * tgaImage -> bpp;
    for (int i = tgaImage -> bpp; i--; ret.bgra[i] = p[i]);
    return ret;
}

void tgaimage_set_tgacolor(TGAImage *tgaImage, int x, int y, TGAColor c) {
    if (tgaImage == NULL || tgaImage -> data == NULL || x < 0 || y < 0 || x >= tgaImage -> width || y >= tgaImage -> height)
        return;
    memcpy(tgaImage -> data + (x + y * tgaImage -> width) * tgaImage -> bpp, c.bgra, tgaImage -> bpp);
}

void tgaimage_flip_horizontally(TGAImage *tgaImage) {
    if(tgaImage == NULL) return;
    int w = tgaImage -> width, h = tgaImage -> height, bpp = tgaImage -> bpp;
    int half = w >> 1, tmp;
    uint8_t *data = tgaImage -> data;
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < h; j++) {
            for (int b = 0; b < bpp; b++) {
                tmp = data[(i + j * w) * bpp + b];
                data[(i + j * w) * bpp + b] = data[(w - 1 - i + j * w) * bpp + b];
                data[(w - 1 - i + j * w) * bpp + b] = tmp;
            }
        }
    }
}

void tgaimage_flip_vertically(TGAImage *tgaImage) {
    if(tgaImage == NULL) return;
    int w = tgaImage -> width, h = tgaImage -> height, bpp = tgaImage -> bpp;
    int half = w >> 1, tmp;
    uint8_t *data = tgaImage -> data;
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < h; j++) {
            for (int b = 0; b < bpp; b++) {
                tmp = data[(i + j * w) * bpp + b];
                data[(i + j * w) * bpp + b] = data[(i + (h - 1 - j) * w) * bpp + b];
                data[(i + (h - 1 - j) * w) * bpp + b] = tmp;
            }
        }
    }
}

static bool tgaimage_load_rle_data(TGAImage *tgaImage, FILE *fp) {
    size_t pixelcount = tgaImage -> width * tgaImage -> height;
    size_t currentpixel = 0;
    size_t currentbyte  = 0;
    TGAColor colorbuffer;
    do {
        uint8_t chunkheader = 0;
        if (fread(&chunkheader, 1, 1, fp) == 0) {
            printf("an error occured while reading the data\n");
            return false;
        }
        if (chunkheader < 128) {
            chunkheader++;
            for (int i = 0; i < chunkheader; i++) {
                if (fread(&colorbuffer.bgra, 1, tgaImage -> bpp, fp) == 0) {
                    printf("an error occured while reading the header\n");
                    return false;
                }
                for (int t = 0; t < tgaImage -> bpp; t++)
                    tgaImage -> data[currentbyte++] = colorbuffer.bgra[t];
                currentpixel++;
                if (currentpixel > pixelcount) {
                    printf("too many pixels read\n");
                    return false;
                }
            }
        } else {
            chunkheader -= 127;
            if (fread(&colorbuffer.bgra, 1, tgaImage -> bpp, fp) == 0) {
                printf("an error occured while reading the header\n");
                return false;
            }
            for (int i = 0; i < chunkheader; i++) {
                for (int t = 0; t < tgaImage -> bpp; t++)
                    tgaImage -> data[currentbyte++] = colorbuffer.bgra[t];
                currentpixel++;
                if (currentpixel > pixelcount) {
                    printf("too many pixels read\n");
                    return false;
                }
            }
        }
    } while (currentpixel < pixelcount);
        return true;
}

// TODO: it is not necessary to break a raw chunk for two equal pixels (for the matter of the resulting size)
static bool tgaimage_unload_rle_data(TGAImage *tgaImage, FILE *fp) {
    uint8_t max_chunk_length = 128;
    size_t npixels = tgaImage -> width * tgaImage -> height;
    size_t curpix = 0;
    while (curpix < npixels) {
        size_t chunkstart = curpix * tgaImage -> bpp;
        size_t curbyte = curpix * tgaImage -> bpp;
        uint8_t run_length = 1;
        bool raw = true;
        while (curpix + run_length < npixels && run_length < max_chunk_length) {
            bool succ_eq = true;
            for (int t = 0; succ_eq && t < tgaImage -> bpp; t++)
                succ_eq = (tgaImage -> data[curbyte + t] == tgaImage -> data[curbyte + t + tgaImage -> bpp]);
            curbyte += tgaImage -> bpp;
            if (1 == run_length)
                raw = !succ_eq;
            if (raw && succ_eq) {
                run_length--;
                break;
            }
            if (!raw && !succ_eq)
                break;
            run_length++;
        }
        curpix += run_length;
        char c = raw ? (run_length - 1) : (run_length + 127);
        if (fwrite(&c, 1, 1, fp) == 0) {
            printf("can't dump the tga file\n");
            return false;
        }
        if (fwrite(tgaImage -> data + chunkstart, tgaImage -> bpp * (raw ? run_length : 1), 1, fp) == 0) {
            printf("can't dump the tga file\n");
            return false;
        }
    }
    return true;
}