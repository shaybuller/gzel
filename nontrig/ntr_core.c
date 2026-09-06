#include "ntr.h"

void ntr_convert_color(color *c){
    c->r = c->r / 255.0f;
    c->g = c->g / 255.0f;
    c->b = c->b / 255.0f;
    c->a = c->a / 255.0f;
}

void ntr_clear_backgorund(color c){
    ntr_convert_color(&c);
    ntr_glClearColor(c.r, c.g, c.b, c.a);
    ntr_glClear(GL_COLOR_BUFFER_BIT);
}