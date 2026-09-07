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
} vec2f32;

typedef struct vec2pf32{
    f32 *x, *y;
} vec2pf32;

typedef struct vec3f32{
    f32 x, y, z;
} vec3f32;

typedef struct vec4f32{
    f32 x, y, z, w;
} vec4f32;

typedef struct mat4f32{
    f32 m[16];
} mat4f32;

float gzel_get_distance(vec2f32 a, vec2f32 b);
float gzel_get_degree(vec2f32 a, vec2f32 b);

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

void gzel_poll_events();
bool gzel_window_xcb_should_close();
void gzel_reset_events();
bool gzel_is_press(i16 key);
bool gzel_is_hold(i16 key);
bool gzel_is_release(i16 key);
void gzel_xcb_set_window(u16 width, u16 height, const char *title);
vec2f32 gzel_get_window_size();

#define gzel_link_gl_platfrom()                gzel_link_gl_xcb()
#define gzel_set_window(width, height, title)  gzel_xcb_set_window(width, height, title)
#define gzel_window_should_close()             gzel_window_xcb_should_close()             

/*----------------------------EMSCRIPTEN-------------------------------------*/
#else

void gzel_link_gl_web();
void gzel_web_set_window(u16 width, u16 height, char *title);
bool gzel_window_web_should_close();

#define gzel_link_gl_platfrom()                gzel_link_gl_web()
#define gzel_set_window(width, height, title)  gzel_web_set_window(width, height, title)
#define gzel_window_should_close()             gzel_window_web_should_close()

#endif

/*--------------------------SHAPE TYPES-----------------------------------*/

typedef struct bool3{
    bool x, y, z;
} bool3;

typedef struct tri_t{
    vec2f32 a, b, c;
} tri_t;

typedef struct box_t{
    f32 x, y; // Position
    f32 w, h; // Dimension
} box_t;

typedef struct poly_t{
    vec2f32 ctr;
    i16 sides;
    f32 radius;
} poly_t;

typedef struct lp_t{ // Line Pointer
    vec2pf32 p1;
    vec2pf32 p2;
    f32 thick;
} lp_t;

/*----------------------------GL-------------------------------------*/

void gzel_egl_swap_buffers();

#ifdef __EMSCRIPTEN__

#include <GLES3/gl3.h>

#define gzel_glEnable                   glEnable
#define gzel_glDisable                  glDisable
#define gzel_glBlendFunc                glBlendFunc
#define gzel_glViewport                 glViewport
#define gzel_glClearColor               glClearColor
#define gzel_glClear                    glClear
#define gzel_glDrawArrays               glDrawArrays
#define gzel_glDrawElements             glDrawElements

#define gzel_glGenBuffers               glGenBuffers
#define gzel_glBindBuffer               glBindBuffer
#define gzel_glBufferData               glBufferData
#define gzel_glDeleteBuffers            glDeleteBuffers

#define gzel_glGenVertexArrays          glGenVertexArrays
#define gzel_glBindVertexArray          glBindVertexArray
#define gzel_glDeleteVertexArrays       glDeleteVertexArrays

#define gzel_glVertexAttribPointer      glVertexAttribPointer
#define gzel_glEnableVertexAttribArray  glEnableVertexAttribArray

#define gzel_glCreateShader             glCreateShader
#define gzel_glShaderSource             glShaderSource
#define gzel_glCompileShader            glCompileShader
#define gzel_glCreateProgram            glCreateProgram
#define gzel_glAttachShader             glAttachShader
#define gzel_glLinkProgram              glLinkProgram
#define gzel_glDeleteShader             glDeleteShader
#define gzel_glDeleteProgram            glDeleteProgram
#define gzel_glUseProgram               glUseProgram

#define gzel_glGetProgramiv             glGetProgramiv
#define gzel_glGetProgramInfoLog        glGetProgramInfoLog

#else

#include <GL/glcorearb.h>
#include <xcb/xcb.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#define GL_FUNCTIONS(X) \
    X(PFNGLENABLEPROC,                   gzel_glEnable,                    glEnable) \
    X(PFNGLDISABLEPROC,                  gzel_glDisable,                   glDisable) \
    X(PFNGLBLENDFUNCPROC,                gzel_glBlendFunc,                 glBlendFunc) \
    X(PFNGLVIEWPORTPROC,                 gzel_glViewport,                  glViewport) \
    X(PFNGLCLEARCOLORPROC,               gzel_glClearColor,                glClearColor) \
    X(PFNGLCLEARPROC,                    gzel_glClear,                     glClear) \
    X(PFNGLDRAWARRAYSPROC,               gzel_glDrawArrays,                glDrawArrays) \
    X(PFNGLDRAWELEMENTSPROC,             gzel_glDrawElements,              glDrawElements) \
    X(PFNGLCREATEBUFFERSPROC,            gzel_glCreateBuffers,             glCreateBuffers) \
    X(PFNGLNAMEDBUFFERSTORAGEPROC,       gzel_glNamedBufferStorage,        glNamedBufferStorage) \
    X(PFNGLBINDVERTEXARRAYPROC,          gzel_glBindVertexArray,           glBindVertexArray) \
    X(PFNGLCREATEVERTEXARRAYSPROC,       gzel_glCreateVertexArrays,        glCreateVertexArrays) \
    X(PFNGLVERTEXARRAYATTRIBBINDINGPROC, gzel_glVertexArrayAttribBinding,    glVertexArrayAttribBinding) \
    X(PFNGLVERTEXARRAYVERTEXBUFFERPROC,  gzel_glVertexArrayVertexBuffer,    glVertexArrayVertexBuffer) \
    X(PFNGLVERTEXARRAYATTRIBFORMATPROC,  gzel_glVertexArrayAttribFormat,     glVertexArrayAttribFormat) \
    X(PFNGLENABLEVERTEXARRAYATTRIBPROC,  gzel_glEnableVertexArrayAttrib,     glEnableVertexArrayAttrib) \
    X(PFNGLCREATESHADERPROC,             gzel_glCreateShader,                glCreateShader) \
    X(PFNGLSHADERSOURCEPROC,             gzel_glShaderSource,                glShaderSource) \
    X(PFNGLCOMPILESHADERPROC,            gzel_glCompileShader,               glCompileShader) \
    X(PFNGLCREATEPROGRAMPROC,            gzel_glCreateProgram,               glCreateProgram) \
    X(PFNGLATTACHSHADERPROC,             gzel_glAttachShader,                glAttachShader) \
    X(PFNGLLINKPROGRAMPROC,              gzel_glLinkProgram,                 glLinkProgram) \
    X(PFNGLDELETESHADERPROC,             gzel_glDeleteShader,                glDeleteShader) \
    X(PFNGLDELETEPROGRAMPROC,            gzel_glDeleteProgram,               glDeleteProgram) \
    X(PFNGLDELETEBUFFERSPROC,            gzel_glDeleteBuffers,               glDeleteBuffers) \
    X(PFNGLDELETEVERTEXARRAYSPROC,       gzel_glDeleteVertexArrays,          glDeleteVertexArrays) \
    X(PFNGLGENVERTEXARRAYSPROC,          gzel_glGenVertexArrays,             glGenVertexArrays) \
    X(PFNGLGENBUFFERSPROC,               gzel_glGenBuffers,                  glGenBuffers) \
    X(PFNGLBINDBUFFERPROC,               gzel_glBindBuffer,                  glBindBuffer) \
    X(PFNGLBUFFERDATAPROC,               gzel_glBufferData,                  glBufferData) \
    X(PFNGLVERTEXATTRIBPOINTERPROC,      gzel_glVertexAttribPointer,         glVertexAttribPointer) \
    X(PFNGLENABLEVERTEXATTRIBARRAYPROC,  gzel_glEnableVertexAttribArray,     glEnableVertexAttribArray) \
    X(PFNGLUSEPROGRAMPROC,               gzel_glUseProgram,                  glUseProgram) \
    X(PFNGLCREATESHADERPROGRAMVPROC,     gzel_glCreateShaderProgramv,        glCreateShaderProgramv) \
    X(PFNGLGETPROGRAMIVPROC,             gzel_glGetProgramiv,                glGetProgramiv) \
    X(PFNGLGETPROGRAMINFOLOGPROC,        gzel_glGetProgramInfoLog,           glGetProgramInfoLog) \
    X(PFNGLGENPROGRAMPIPELINESPROC,      gzel_glGenProgramPipelines,         glGenProgramPipelines) \
    X(PFNGLUSEPROGRAMSTAGESPROC,         gzel_glUseProgramStages,            glUseProgramStages) \
    X(PFNGLBINDPROGRAMPIPELINEPROC,      gzel_glBindProgramPipeline,         glBindProgramPipeline) \
    X(PFNGLPROGRAMUNIFORMMATRIX2FVPROC,  gzel_glProgramUniformMatrix2fv,     glProgramUniformMatrix2fv) \
    X(PFNGLBINDTEXTUREUNITPROC,          gzel_glBindTextureUnit,             glBindTextureUnit) \
    X(PFNGLCREATETEXTURESPROC,           gzel_glCreateTextures,              glCreateTextures) \
    X(PFNGLTEXTUREPARAMETERIPROC,        gzel_glTextureParameteri,           glTextureParameteri) \
    X(PFNGLTEXTURESTORAGE2DPROC,         gzel_glTextureStorage2D,            glTextureStorage2D) \
    X(PFNGLTEXTURESUBIMAGE2DPROC,        gzel_glTextureSubImage2D,           glTextureSubImage2D) \
    X(PFNGLDEBUGMESSAGECALLBACKPROC,     gzel_glDebugMessageCallback,        glDebugMessageCallback)

#define X(type, gzel_name, gl_name) extern type gzel_name;
GL_FUNCTIONS(X)
#undef X

extern EGLDisplay display;
extern EGLContext context;
extern EGLSurface surface;

void gzel_link_gl_xcb();
void gzel_disconnect_gl(void);

#endif

// core
void gzel_convert_color    (color *c);
void gzel_clear_backgorund (color c);
void gzel_start_cycle();

#define gzel_end_cycle() gzel_reset_events()

// shapes
extern const char *vertexShaderSource;

void gzel_tri      (vec2f32 a, vec2f32 b, vec2f32 c, color col);
void gzel_line     (vec2f32 start, vec2f32 end, f32 thick, color col);
void gzel_box      (i16 x, i16 y, u16 width, u16 height, color col);
void gzel_box_box  (box_t box, color col);
void gzel_poly     (vec2f32 ctr, u8 sides, f32 radius, f32 rot, color col);

bool gzel_collide_pt_box     (vec2f32 pt, box_t box);
bool gzel_collide_pt_tri     (vec2f32 pt, vec2f32 a, vec2f32 b, vec2f32 c);
bool gzel_collide_pt_poly    (vec2f32 pt, vec2f32 ctr, u16 sides, f32 radius);
bool gzel_collide_pt_line    (vec2f32 pt, vec2f32 start, vec2f32 end, f32 thick);

// texts
void gzel_draw_text(const c8 *text, u16 pos_x, u16 pos_y, u16 font_size, color col);