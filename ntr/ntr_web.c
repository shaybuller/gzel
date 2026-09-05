#include "ntr.h"
/*
void ntr_link_gl_web()
{
    EmscriptenWebGLContextAttributes attr;
    emscripten_webgl_init_context_attributes(&attr);

    attr.majorVersion = 2;
    attr.minorVersion = 0;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attr);


    emscripten_webgl_make_context_current(ctx);
}

void ntr_web_set_window(u16 width, u16 height, char *title){
     EM_ASM({
        document.title = UTF8ToString($0);

        const canvas = document.getElementById("canvas");
        canvas.width = $1;
        canvas.height = $2;
    }, title, width, height);
}

bool ntr_window_web_should_close(){
    return false;
}
*/