#include "gzel.h"

void gzel_egl_swap_buffers()
{
    #ifndef __EMSCRIPTEN__ 

    eglSwapBuffers(display, surface);

    #endif

}