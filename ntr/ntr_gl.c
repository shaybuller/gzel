#include "ntr.h"

void ntr_egl_swap_buffers()
{
    #ifndef __EMSCRIPTEN__ 

    eglSwapBuffers(display, surface);

    #endif

}