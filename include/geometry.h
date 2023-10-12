//
// Created by taihangindra on 2023/6/13.
//

#ifndef PICORENDERER_GEOMETRY_H
#define PICORENDERER_GEOMETRY_H

typedef struct {
    union {
        struct {float x, y;};
        struct {float u, v;};
        float raw[2];
    };
}Vec2f;

typedef struct {
    union {
        struct {int x, y;};
        struct {int u, v;};
        int raw[2];
    };
}Vec2i;

typedef struct {
    union {
        struct {float x, y, z;};
        struct {float ivert, iuv, inorm;};
        float raw[3];
    };
}Vec3f;

typedef struct {
    union {
        struct {int x, y, z;};
        struct {int ivert, iuv, inorm;};
        int raw[3];
    };
}Vec3i;

Vec2f vec2f_add(Vec2f v1, Vec2f v2) { return (Vec2f){v1.x + v2.x, v1.y + v2.y}; }
Vec2i vec2i_add(Vec2i v1, Vec2i v2) { return (Vec2i){v1.x + v2.x, v1.y + v2.y}; }

Vec2f vec2f_sub(Vec2f v1, Vec2f v2) { return (Vec2f){v1.x - v2.x, v1.y - v2.y}; }
Vec2i vec2i_sub(Vec2i v1, Vec2i v2) { return (Vec2i){v1.x - v2.x, v1.y - v2.y}; }

Vec2f vec2f_muptiply(Vec2f v1, Vec2f v2) { return (Vec2f){v1.x * v2.x, v1.y * v2.y}; }
Vec2i vec2i_multiply(Vec2i v1, Vec2i v2) { return (Vec2i){v1.x * v2.x, v1.y * v2.y}; }

Vec2f vec2f_muptiply_scalar(Vec2f v, float scalar) { return (Vec2f){v.x * scalar, v.y * scalar}; }
Vec2i vec2i_multiply_scalar(Vec2i v, float scalar) { return (Vec2i){(int)(v.x * scalar), (int)(v.y * scalar)}; }

Vec2f vec2f_devide(Vec2f v1, Vec2f v2) { return (Vec2f){v1.x / v2.x, v1.y / v2.y}; }
Vec2i vec2i_devide(Vec2i v1, Vec2i v2) { return (Vec2i){v1.x / v2.x, v1.y / v2.y}; }

float vec2f_dot(Vec2f v1, Vec2f v2) { return v1.x * v2.x + v1.y * v2.y; }
int vec2i_dot(Vec2i v1, Vec2i v2) { return v1.x * v2.x + v1.y * v2.y; }



Vec3f vec3f_add(Vec3f v1, Vec3f v2) { return (Vec3f){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }
Vec3i vec3i_add(Vec3i v1, Vec3i v2) { return (Vec3i){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }

Vec3f vec3f_sub(Vec3f v1, Vec3f v2) { return (Vec3f){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; }
Vec3i vec3i_sub(Vec3i v1, Vec3i v2) { return (Vec3i){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; }

Vec3f vec3f_muptiply(Vec3f v1, Vec3f v2) { return (Vec3f){v1.x * v2.x, v1.y * v2.y, v1.z * v2.z}; }
Vec3i vec3i_multiply(Vec3i v1, Vec3i v2) { return (Vec3i){v1.x * v2.x, v1.y * v2.y, v1.z * v2.z}; }

Vec3f vec3f_cross(Vec3f v1, Vec3f v2) { return (Vec3f){v1.y* v2.z - v1.z * v2.y, v1.z* v2.x - v1.x * v2.z, v1.x* v2.y - v1.y * v2.x}; }
Vec3i vec3i_cross(Vec3i v1, Vec3i v2) { return (Vec3i){v1.y* v2.z - v1.z * v2.y, v1.z* v2.x - v1.x * v2.z, v1.x* v2.y - v1.y * v2.x}; }

Vec3f vec3f_devide(Vec3f v1, Vec3f v2) { return (Vec3f){v1.x / v2.x, v1.y / v2.y, v1.z / v2.z}; }
Vec3i vec3i_devide(Vec3i v1, Vec3i v2) { return (Vec3i){v1.x / v2.x, v1.y / v2.y, v1.z / v2.z}; }

float vec3f_dot(Vec3f v1, Vec3f v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
int vec3i_dot(Vec3i v1, Vec3i v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

#endif //PICORENDERER_GEOMETRY_H
