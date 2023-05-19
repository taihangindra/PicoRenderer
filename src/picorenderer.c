//
// Created by taihangindra on 2023/5/18.
//
#include "../include/picorenderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

model_t *model_load(char *object_name, char *texture_name);

picorenderer_t* picorenderer_init (int width, int height, char *object_name, char *texture_name)
{
    picorenderer_t *pr = calloc(1, sizeof(picorenderer_t));
    if(!pr) goto failed;
    model_t *model = model_load(object_name, texture_name);
    if(!model) goto failed;
    FILE *fp = fopen(object_name, "rb");


    return pr;

failed:
    picorenderer_free(pr);
    return NULL;
}
void picorenderer_free(picorenderer_t *pr){
    if(pr) {

        free(pr);
    }
}
void picorenderer_begin(picorenderer_t *pr, int clear);
void picorenderer_end(picorenderer_t *pr);
void picorenderer_draw(picorenderer_t *pr);
void picorenderer_clear(picorenderer_t *pr, char *type);
void picorenderer_viewport(picorenderer_t *pr, int x, int y, int w, int h, int depth);
void picorenderer_set(picorenderer_t *pr, char *name, void *data);
void* picorenderer_get(picorenderer_t *pr, char *name);

model_t *model_load(char *object_name, char *texture_name){
    model_t *model = NULL;
    int v_num = 0, vt_num = 0, vn_num = 0, f_num = 0;
    FILE *fp = fopen(object_name, "rb");
    char buf[64];
    if(!fp) { printf("failed to open model file %s!\n", object_name); goto failed; }

    // 获取 object_name 中所包含的 v / vt / vn 的数目
    while(!feof(fp)){
        fscanf(fp, "%63s", buf);
        if(strcmp(buf, "v") == 0) v_num++;
        else if(strcmp(buf, "vt") == 0) vt_num++;
        else if(strcmp(buf, "vn") == 0) vn_num++;
        else if(strcmp(buf, "f") == 0) f_num++;
        fgets(buf, sizeof(buf), fp);
    }

    // 为 model 分配足够的空间，除 model 本身的空间外，需要预留出 .obj 文件中所包含的数据
    model = calloc(1, sizeof(model_t) + 3 * (v_num + vt_num + vn_num) * sizeof(float) + 3 * 3 * f_num * sizeof(int));
    if(!model) { printf("memory error!\n"); goto failed; }

    model->v = (vec3f_t *)(model + 1);
    model->vt = (vec3f_t *)(model->v + v_num);
    model->vn = (vec3f_t *)(model->vt + vt_num);
    model->f = (face_t *)(model->vn + vn_num);
    // 将 fp 的文件指针清零，开始读取具体的 v / vt / vn 数据
    fseek(fp, 0, SEEK_SET);
    while (!feof(fp)) {
        fscanf(fp, "%63s", buf);
        if (strcmp(buf, "v" ) == 0 && model->v_num < v_num) {
            fscanf(fp, "%f %f %f", &(model->v[model->v_num].x), &(model->v[model->v_num].y), &(model->v[model->v_num].z));
            model->v_num++;
        } else if (strcmp(buf, "vt") == 0 && model->vt_num < vt_num) {
            fscanf(fp, "%f %f %f", &(model->vt[model->vt_num].x), &(model->vt[model->vt_num].y), &(model->vt[model->vt_num].z));
            model->vt_num++;
        } else if (strcmp(buf, "vn") == 0 && model->vn_num < vn_num) {
            fscanf(fp, "%f %f %f", &(model->vn[model->vn_num].x), &(model->vn[model->vn_num].y), &(model->vn[model->vn_num].z));
            model->vn_num++;
        } else if (strcmp(buf, "f" ) == 0 && model->f_num  < f_num ) {
            fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d",
                   &(model->f[model->f_num].v[0]), &(model->f[model->f_num].vt[0]), &(model->f[model->f_num].vn[0]),
                   &(model->f[model->f_num].v[1]), &(model->f[model->f_num].vt[1]), &(model->f[model->f_num].vn[1]),
                   &(model->f[model->f_num].v[2]), &(model->f[model->f_num].vt[2]), &(model->f[model->f_num].vn[2]));
            model->f_num++;
        }
    }

    return model;

failed:
    fclose(fp);
    return NULL;
}