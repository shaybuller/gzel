#include "gzel.h"

void gzel_convert_color(color *c){
    c->r = c->r / 255.0f;
    c->g = c->g / 255.0f;
    c->b = c->b / 255.0f;
    c->a = c->a / 255.0f;
}

void gzel_clear_backgorund(color c){
    gzel_convert_color(&c);
    gzel_glClearColor(c.r, c.g, c.b, c.a);
    gzel_glClear(GL_COLOR_BUFFER_BIT);
}