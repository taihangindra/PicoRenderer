picorenderer
--------
--------
tinygl.h
--------
用于渲染模型，创建后内部会创建一个 texture 对象作为 render target
也可以通过 tinygl_set(gl, "target", texture); 来指定一个新的 texture 作为 target
通过 tinygl_set(gl, "save", “out.bmp”); 可以把渲染结果保存到文件进行查看

void* tinygl_init(int w, int h);
初始化 tinygl 并得到 tinygl 的对象指针

void tinygl_free(void *ctx);
反初始化

void tinygl_begin(void *ctx, int clear);
开始渲染，clear 如果为 1 会清除 framebuffer 和 zbuffer

void tinygl_end(void *ctx);
完成渲染

void tinygl_draw(void *ctx, void *model);
放到 tinygl_begin 和 tinygl_end 中间，用于绘制模型

void tinygl_clear(void *ctx, char *type);
用于清除 frame buffer 或者 z-buffer
tinygl_clear(gl, "framebuf"); // 清除 frame buffer
tinygl_clear(gl, "zbuffer" ); // 清除 z-buffer
tinygl_clear(gl, "framebuf+zbuffer"); // 同时清除 frame buffer 和 z-buffer

void  tinygl_set(void *ctx, char *name, void *data);
void* tinygl_get(void *ctx, char *name);
用于获取和设置渲染参数