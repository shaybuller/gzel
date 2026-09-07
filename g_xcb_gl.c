#ifndef __EMSCRIPTEN__

#include "gzel.h"

#define X(type,ntr_name,gl_name) type ntr_name;
GL_FUNCTIONS(X)
#undef X

EGLDisplay display;
EGLContext context;
EGLSurface surface;

static PFNEGLGETPLATFORMDISPLAYEXTPROC GetPlatformDisplayEXT;
static PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC CreatePlatformWindowSurfaceEXT;


void gzel_link_gl_xcb()
{
    // Load openGL functions from pfn
    #define X(type,ntr_name,gl_name) \
        ntr_name = (type)eglGetProcAddress(#gl_name);
        GL_FUNCTIONS(X)
    #undef X

    GetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");

    CreatePlatformWindowSurfaceEXT = (PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC)eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT");

    display = GetPlatformDisplayEXT(
        EGL_PLATFORM_XCB_EXT, connection,
        (const EGLint[]){
            EGL_PLATFORM_XCB_SCREEN_EXT, 0,
            EGL_NONE
        }
    );

    EGLint major, minor;
    eglInitialize(display, &major, &minor);
    eglBindAPI(EGL_OPENGL_API);

    EGLConfig config;
    EGLint config_attributes[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_NONE
    };

    EGLint config_count;

    eglChooseConfig(
        display,
        config_attributes,
        &config,
        1,
        &config_count
    );

    surface = CreatePlatformWindowSurfaceEXT(
        display,
        config,
        &window,
        (const EGLint[]){
            EGL_GL_COLORSPACE, EGL_GL_COLORSPACE_LINEAR,
            EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
            EGL_NONE
        }
    );

    context = eglCreateContext(
        display,
        config,
        EGL_NO_CONTEXT,
        (const EGLint[]){
            EGL_CONTEXT_MAJOR_VERSION, 4,
            EGL_CONTEXT_MINOR_VERSION, 5,
            EGL_CONTEXT_OPENGL_PROFILE_MASK,
            EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
            EGL_NONE
        }
    );

    eglMakeCurrent(display, surface, surface, context);
    eglSwapInterval(display, 1);
    vec2f32 size = gzel_get_window_size();
    gzel_glViewport(0, 0, size.x, size.y);
}

void gzel_disconnect_gl()
{
    eglMakeCurrent(
        display,
        EGL_NO_SURFACE,
        EGL_NO_SURFACE,
        EGL_NO_CONTEXT
    );

    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
}

#endif