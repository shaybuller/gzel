#pragma once

#include <math.h>
#include <uchar.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <xcb/xcb.h>
#define explicit explicit_bug_workaround
    #include <xcb/xkb.h>
#undef explicit

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GL/glcorearb.h>

/*----------------------------TYPES-------------------------------------*/
 
typedef int8_t  i8;    // -128 to 127
typedef int16_t i16;   // -32,768 to 32,767
typedef int32_t i32;   // -2,147,483,648 to 2,147,483,647
typedef int64_t i64;   // -9,223,372,036,854,775,808
                       // to  9,223,372,036,854,775,807
typedef uint8_t  u8;   // 0 to 255
typedef uint16_t u16;  // 0 to 65,535
typedef uint32_t u32;  // 0 to 4,294,967,295
typedef uint64_t u64;  // 0 to 18,446,744,073,709,551,615

typedef float  f32;
typedef double f64;

typedef char          c8;
typedef unsigned char uc8;

typedef size_t    usize;
typedef ptrdiff_t isize;

/*----------------------------MATH-------------------------------------*/

#define PI 3.14159265358979323846

typedef struct vec2f32 {
    f32 x, y;
}vec2f32;

typedef struct vec2pf32{
    f32 *x ,*y;
}vec2pf32;

typedef struct vec3f32{
    f32 x, y, z;
}vec3f32;


typedef struct vec4f32{
    f32 x, y, z, w;
}vec4f32;

typedef struct mat4f32{
    f32 m[16];
}mat4f32;


float ntr_get_distance(vec2f32 a, vec2f32 b);
float ntr_get_degree(vec2f32 a, vec2f32 b);

/*----------------------------Color-------------------------------------*/

typedef struct color {
    f32 r, g, b, a;
} color;

#define LIGHTGRAY    (color){200,200,200,255}
#define GRAY         (color){130,130,130,255}
#define DARKGRAY     (color){80,80,80,255}
#define CHARCOAL     (color){54,69,79,255}
#define DARKCHARCOAL (color){48,48,48,255}
#define YELLOW       (color){253,249,0,255}
#define GOLD         (color){255,203,0,255}
#define ORANGE       (color){255,161,0,255}
#define PINK         (color){255,109,194,255}
#define RED          (color){230,41,55,255}
#define MAROON       (color){190,33,55,255}
#define GREEN        (color){0,228,48,255}
#define LIME         (color){0,158,47,255}
#define DARKGREEN    (color){0,117,44,255}
#define SKYBLUE      (color){102,191,255,255}
#define CYAN         (color){0,255,255,255}
#define BLUE         (color){0,121,241,255}
#define DARKBLUE     (color){0,82,172,255}
#define PURPLE       (color){200,122,255,255}
#define VIOLET       (color){135,60,190,255}
#define DARKPURPLE   (color){112,31,126,255}
#define BEIGE        (color){211,176,131,255}
#define BROWN        (color){127,106,79,255}
#define DARKBROWN    (color){76,63,47,255}
#define WHITE        (color){255,255,255,255}
#define WHITESMOKE   (color){245,245,245,255}
#define BLACK        (color){0,0,0,255}
#define BLANK        (color){0,0,0,0}
#define MAGENTA      (color){255,0,255,255}

#ifndef __EMSCRIPTEN__
/*----------------------------XCB-------------------------------------*/

typedef enum {
    M_BTN_LEFT = 1,
    M_WHEEL = 2,
    M_BTN_RIGHT = 3,
    M_WHEEL_UP = 4,
    M_WHEEL_DOWN = 5
} MouseBtn_xcb;

typedef enum {
    K_1=10, K_2=11, K_3=12, K_4=13, K_5=14,
    K_6=15, K_7=16, K_8=17, K_9=18, K_0=19,

    K_A=38, K_B=56, K_C=54, K_D=40, K_E=26,
    K_F=41, K_G=42, K_H=43, K_I=31, K_J=44,
    K_K=45, K_L=46, K_M=58, K_N=57, K_O=32,
    K_P=33, K_Q=24, K_R=27, K_S=39, K_T=28,
    K_U=30, K_V=55, K_W=25, K_X=53, K_Y=29,
    K_Z=52,

    K_BACKSPACE = 22,
    K_SHIFT     = 50
} Keyboard_xcb;

extern xcb_generic_event_t *event;
extern xcb_window_t window;
extern xcb_connection_t *connection;
extern i16 MOUSE_PRESS_SIGN;
extern i16 MOUSE_HOLD_SIGN;
extern i16 MOUSE_RELEASE_SIGN;
extern i16 KEY_PRESS_SIGN;
extern i16 KEY_HOLD_SIGN;
extern i16 KEY_RELEASE_SIGN;
extern vec2f32 mpos;
extern vec2f32 press_pos;

void ntr_poll_events();
bool ntr_window_xcb_should_close();
void ntr_reset_events();
bool ntr_is_press(i16 key);
bool ntr_is_hold(i16 key);
bool ntr_is_release(i16 key);
void ntr_xcb_set_window(u16 width, u16 height, const char *title);
vec2f32 ntr_get_window_size();

#define ntr_link_gl_platfrom()                 ntr_link_gl_xcb()
#define ntr_set_window(width, height, title)   ntr_xcb_set_window(width, height, title)
#define ntr_window_should_close()              ntr_window_xcb_should_close()             

/*----------------------------EMSCRIPTEN-------------------------------------*/
#else

void ntr_link_gl_web();
void ntr_web_set_window(u16 width, u16 height, char *title);
bool ntr_window_web_should_close();

#define ntr_link_gl_platfrom()                 ntr_link_gl_web()
#define ntr_set_window(width, height, title)   ntr_web_set_window(width, height, title)
#define ntr_window_should_close()              ntr_window_web_should_close()

#endif

/*--------------------------SHAPE TYPES-----------------------------------*/

typedef struct bool3{
    bool x, y, z;
}bool3;

typedef struct tri_t{
    vec2f32 a, b, c;
}tri_t;

typedef struct box_t{
    f32 x ,y; // Postion
    f32 w ,h; // Dimention
}box_t;

typedef struct poly_t{
    vec2f32    ctr;
    i16   sides;
    f32 radius;
}poly_t;

typedef struct lp_t{ // Line Pointer
    vec2pf32 p1;
    vec2pf32 p2;
    f32 thick;
}lp_t;


/*----------------------------GL-------------------------------------*/

void ntr_egl_swap_buffers();

#ifdef __EMSCRIPTEN__

#include <GLES3/gl3.h>

#define ntr_glEnable                    glEnable
#define ntr_glDisable                   glDisable
#define ntr_glBlendFunc                 glBlendFunc
#define ntr_glViewport                  glViewport
#define ntr_glClearColor                glClearColor
#define ntr_glClear                     glClear
#define ntr_glDrawArrays                glDrawArrays
#define ntr_glDrawElements              glDrawElements

#define ntr_glGenBuffers                glGenBuffers
#define ntr_glBindBuffer                glBindBuffer
#define ntr_glBufferData                glBufferData
#define ntr_glDeleteBuffers             glDeleteBuffers

#define ntr_glGenVertexArrays           glGenVertexArrays
#define ntr_glBindVertexArray            glBindVertexArray
#define ntr_glDeleteVertexArrays        glDeleteVertexArrays

#define ntr_glVertexAttribPointer       glVertexAttribPointer
#define ntr_glEnableVertexAttribArray   glEnableVertexAttribArray

#define ntr_glCreateShader              glCreateShader
#define ntr_glShaderSource              glShaderSource
#define ntr_glCompileShader             glCompileShader
#define ntr_glCreateProgram             glCreateProgram
#define ntr_glAttachShader              glAttachShader
#define ntr_glLinkProgram               glLinkProgram
#define ntr_glDeleteShader              glDeleteShader
#define ntr_glDeleteProgram             glDeleteProgram
#define ntr_glUseProgram                glUseProgram

#define ntr_glGetProgramiv              glGetProgramiv
#define ntr_glGetProgramInfoLog         glGetProgramInfoLog

#else

#include <GL/glcorearb.h>
#include <xcb/xcb.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#define GL_FUNCTIONS(X) \
    X(PFNGLENABLEPROC,                   ntr_glEnable,                    glEnable) \
    X(PFNGLDISABLEPROC,                  ntr_glDisable,                   glDisable) \
    X(PFNGLBLENDFUNCPROC,                ntr_glBlendFunc,                 glBlendFunc) \
    X(PFNGLVIEWPORTPROC,                 ntr_glViewport,                  glViewport) \
    X(PFNGLCLEARCOLORPROC,               ntr_glClearColor,                glClearColor) \
    X(PFNGLCLEARPROC,                    ntr_glClear,                     glClear) \
    X(PFNGLDRAWARRAYSPROC,               ntr_glDrawArrays,                glDrawArrays) \
    X(PFNGLDRAWELEMENTSPROC,             ntr_glDrawElements,              glDrawElements) \
    X(PFNGLCREATEBUFFERSPROC,            ntr_glCreateBuffers,              glCreateBuffers) \
    X(PFNGLNAMEDBUFFERSTORAGEPROC,       ntr_glNamedBufferStorage,         glNamedBufferStorage) \
    X(PFNGLBINDVERTEXARRAYPROC,          ntr_glBindVertexArray,             glBindVertexArray) \
    X(PFNGLCREATEVERTEXARRAYSPROC,       ntr_glCreateVertexArrays,          glCreateVertexArrays) \
    X(PFNGLVERTEXARRAYATTRIBBINDINGPROC, ntr_glVertexArrayAttribBinding,     glVertexArrayAttribBinding) \
    X(PFNGLVERTEXARRAYVERTEXBUFFERPROC,  ntr_glVertexArrayVertexBuffer,     glVertexArrayVertexBuffer) \
    X(PFNGLVERTEXARRAYATTRIBFORMATPROC,  ntr_glVertexArrayAttribFormat,      glVertexArrayAttribFormat) \
    X(PFNGLENABLEVERTEXARRAYATTRIBPROC,  ntr_glEnableVertexArrayAttrib,      glEnableVertexArrayAttrib) \
    X(PFNGLCREATESHADERPROC,             ntr_glCreateShader,                 glCreateShader) \
    X(PFNGLSHADERSOURCEPROC,             ntr_glShaderSource,                 glShaderSource) \
    X(PFNGLCOMPILESHADERPROC,            ntr_glCompileShader,                glCompileShader) \
    X(PFNGLCREATEPROGRAMPROC,            ntr_glCreateProgram,                glCreateProgram) \
    X(PFNGLATTACHSHADERPROC,             ntr_glAttachShader,                 glAttachShader) \
    X(PFNGLLINKPROGRAMPROC,              ntr_glLinkProgram,                  glLinkProgram) \
    X(PFNGLDELETESHADERPROC,             ntr_glDeleteShader,                 glDeleteShader) \
    X(PFNGLDELETEPROGRAMPROC,            ntr_glDeleteProgram,                glDeleteProgram) \
    X(PFNGLDELETEBUFFERSPROC,            ntr_glDeleteBuffers,                glDeleteBuffers) \
    X(PFNGLDELETEVERTEXARRAYSPROC,       ntr_glDeleteVertexArrays,            glDeleteVertexArrays) \
    X(PFNGLGENVERTEXARRAYSPROC,          ntr_glGenVertexArrays,               glGenVertexArrays) \
    X(PFNGLGENBUFFERSPROC,               ntr_glGenBuffers,                    glGenBuffers) \
    X(PFNGLBINDBUFFERPROC,               ntr_glBindBuffer,                    glBindBuffer) \
    X(PFNGLBUFFERDATAPROC,               ntr_glBufferData,                    glBufferData) \
    X(PFNGLVERTEXATTRIBPOINTERPROC,      ntr_glVertexAttribPointer,           glVertexAttribPointer) \
    X(PFNGLENABLEVERTEXATTRIBARRAYPROC,  ntr_glEnableVertexAttribArray,       glEnableVertexAttribArray) \
    X(PFNGLUSEPROGRAMPROC,               ntr_glUseProgram,                    glUseProgram) \
    X(PFNGLCREATESHADERPROGRAMVPROC,     ntr_glCreateShaderProgramv,          glCreateShaderProgramv) \
    X(PFNGLGETPROGRAMIVPROC,             ntr_glGetProgramiv,                  glGetProgramiv) \
    X(PFNGLGETPROGRAMINFOLOGPROC,        ntr_glGetProgramInfoLog,              glGetProgramInfoLog) \
    X(PFNGLGENPROGRAMPIPELINESPROC,      ntr_glGenProgramPipelines,            glGenProgramPipelines) \
    X(PFNGLUSEPROGRAMSTAGESPROC,         ntr_glUseProgramStages,               glUseProgramStages) \
    X(PFNGLBINDPROGRAMPIPELINEPROC,      ntr_glBindProgramPipeline,            glBindProgramPipeline) \
    X(PFNGLPROGRAMUNIFORMMATRIX2FVPROC,  ntr_glProgramUniformMatrix2fv,        glProgramUniformMatrix2fv) \
    X(PFNGLBINDTEXTUREUNITPROC,          ntr_glBindTextureUnit,                glBindTextureUnit) \
    X(PFNGLCREATETEXTURESPROC,           ntr_glCreateTextures,                 glCreateTextures) \
    X(PFNGLTEXTUREPARAMETERIPROC,        ntr_glTextureParameteri,              glTextureParameteri) \
    X(PFNGLTEXTURESTORAGE2DPROC,         ntr_glTextureStorage2D,               glTextureStorage2D) \
    X(PFNGLTEXTURESUBIMAGE2DPROC,        ntr_glTextureSubImage2D,               glTextureSubImage2D) \
    X(PFNGLDEBUGMESSAGECALLBACKPROC,     ntr_glDebugMessageCallback,            glDebugMessageCallback)

#define X(type, ntr_name, gl_name) extern type ntr_name;
GL_FUNCTIONS(X)
#undef X

extern EGLDisplay display;
extern EGLContext context;
extern EGLSurface surface;

void ntr_link_gl_xcb();
void ntr_disconnect_gl(void);

#endif

// core
void ntr_convert_color    (color *c);
void ntr_clear_backgorund (color c);

// shapes
extern const char *vertexShaderSource;

void ntr_tri      (vec2f32 a, vec2f32 b, vec2f32 c, color col);
void ntr_line     (vec2f32 start, vec2f32 end, f32 thick, color col);
void ntr_box      (i16 x, i16 y, u16 width, u16 height, color col);
void ntr_box_box  (box_t box, color col);
void ntr_poly     (vec2f32 ctr, u8 sides, f32 radius, f32 rot, color col);

bool ntr_collide_pt_box     (vec2f32 pt, box_t box);
bool ntr_collide_pt_tri     (vec2f32 pt, vec2f32 a ,vec2f32 b ,vec2f32 c);
bool ntr_collide_pt_poly    (vec2f32 pt, vec2f32 ctr, u16 sides, f32 radius);
bool ntr_collide_pt_line    (vec2f32 pt, vec2f32 start, vec2f32 end, f32 thick);

// fonts 

void ntr_draw_text(const c8 *text, u16 pos_x, u16 pos_y, u16 font_size, color col);