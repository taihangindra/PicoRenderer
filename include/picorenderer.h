//
// Created by taihangindra on 2023/5/18.
//

#ifndef PICORENDERER_PICORENDERER_H
#define PICORENDERER_PICORENDERER_H

#include "vector.h"

typedef struct{
    int v[3], vt[3], vn[3];
}face_t;

typedef struct{
    int v_num, vt_num, vn_num, f_num;
    vec3f_t *v, *vt, *vn;
    face_t *f;
}model_t;

typedef struct{
    model_t *model;
}picorenderer_t;

picorenderer_t* picorenderer_init (int width, int height, char *object_name, char *texture_name);
void picorenderer_free(picorenderer_t *pr);
void picorenderer_begin(picorenderer_t *pr, int clear);
void picorenderer_end(picorenderer_t *pr);
void picorenderer_draw(picorenderer_t *pr);
void picorenderer_clear(picorenderer_t *pr, char *type);
void picorenderer_viewport(picorenderer_t *pr, int x, int y, int w, int h, int depth);
void picorenderer_set(picorenderer_t *pr, char *name, void *data);
void* picorenderer_get(picorenderer_t *pr, char *name);

#endif //PICORENDERER_PICORENDERER_H
