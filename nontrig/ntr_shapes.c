#include "ntr.h"
#include <stdarg.h>
#include <math.h>
#include <string.h>

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main(){\n"
    "    gl_Position   = vec4(aPos, 1.0);\n"
    "}\n";

const char *ntr_frag_shader_src(color c)
{
    ntr_convert_color(&c);
    static c8 src[512];
    snprintf(src, sizeof(src),
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "    FragColor = vec4(%f,%f,%f,%f);\n"
        "}\n",
        c.r, c.g, c.b, c.a);
    return src;
}

void ntr_map_floats(i16 count, ...)
{
    vec2f32 size = ntr_get_window_size();
    va_list args;
    va_start(args, count);
    for (u16 i = 0; i < count; i++) {
        f32 * p = va_arg(args, f32 *);
        if (i % 2 == 0) (*p) = (*p) / size.x * 2 - 1; // Even means X value
        else (*p) = (*p) / size.y * -2 + 1;                // Odd  means Y value
    }
    va_end(args);
}

void ntr_create_program(uint32_t *program,const char *vs_src ,const char *fs_src)
{
    uint32_t vs = ntr_glCreateShader(GL_VERTEX_SHADER);
    ntr_glShaderSource(vs, 1, &vs_src, NULL);
    ntr_glCompileShader(vs);

    uint32_t fs = ntr_glCreateShader(GL_FRAGMENT_SHADER);
    ntr_glShaderSource(fs, 1, &fs_src, NULL);
    ntr_glCompileShader(fs);

    *program = ntr_glCreateProgram();
    ntr_glAttachShader(*program, vs);
    ntr_glAttachShader(*program, fs);
    ntr_glLinkProgram(*program);

    ntr_glDeleteShader(vs);
    ntr_glDeleteShader(fs);
}

void ntr_tri(vec2f32 a, vec2f32 b, vec2f32 c, color col)
{
    ntr_map_floats(6, &a.x ,&a.y, &b.x, &b.y, &c.x, &c.y);

    f32 vertices[] = {
        a.x,a.y,0,
        b.x,b.y,0,
        c.x,c.y,0
    };

    uint32_t program,VAO, VBO;
    ntr_create_program(&program,vertexShaderSource,ntr_frag_shader_src(col));

    ntr_glGenVertexArrays(1, &VAO);
    ntr_glGenBuffers(1, &VBO);

    ntr_glBindVertexArray(VAO);

    ntr_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ntr_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ntr_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3 * sizeof(f32), (f32 *)0);
    ntr_glEnableVertexAttribArray(0);

    ntr_glBindBuffer(GL_ARRAY_BUFFER, 0);
    ntr_glBindVertexArray(0);

    ntr_glUseProgram(program);
    ntr_glBindVertexArray(VAO);
    ntr_glDrawArrays(GL_TRIANGLES, 0, 3);
        // Delete allocations
    ntr_glDeleteVertexArrays(1, &VAO);
    ntr_glDeleteBuffers(1, &VBO);
    ntr_glDeleteProgram(program);
}

void ntr_line(vec2f32 start, vec2f32 end, f32 thick, color col)
{   
    f32 dx, dy, m, deg;
    vec2f32 s2, e2;

    dx = end.x - start.x;
    dy = end.y - start.y;
    if(dx) {
        m = dy / dx;
        deg = atanf(m) * 180 / PI;
        if(m < 0) deg = -deg;            // Slope negaive (to us it will look postive)
        if(m){
            s2.x = start.x + thick * cosf(fmodf(deg + 270.0f, 360.0f));
            s2.y = start.y - thick * sinf(fmodf(deg + 270.0f, 360.0f));

            e2.x = end.x + thick * cosf(fmodf(deg + 270.0f, 360.0f));
            e2.y = end.y - thick * sinf(fmodf(deg + 270.0f, 360.0f));
        }
        else{
            s2 = (vec2f32){start.x, start.y - thick};
            e2 = (vec2f32){end.x, end.y - thick};
        }
    }
    else{
            s2 = (vec2f32){start.x + thick, start.y};
            e2 = (vec2f32){end.x + thick, end.y};
    }

    f32 vertices[] = {
        start.x,       start.y, 0,
        s2.x,          s2.y,    0,
        end.x,         end.y,   0,
        e2.x,          e2.y,    0
    };

    ntr_map_floats(8, &vertices[0], &vertices[1], &vertices[3], &vertices[4],
                      &vertices[6], &vertices[7], &vertices[9], &vertices[10]);

    u32 indices[] = {
        0,1,3,
        2,3,0
    };

    u32 program, VAO, VBO, EBO;
    ntr_create_program(&program,vertexShaderSource,ntr_frag_shader_src(col));

    ntr_glGenVertexArrays(1, &VAO);
    ntr_glGenBuffers(1, &VBO);
    ntr_glGenBuffers(1, &EBO);

    ntr_glBindVertexArray(VAO);

    ntr_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ntr_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    ntr_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    ntr_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);

    ntr_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3 * sizeof(f32), (void *)0);
    ntr_glEnableVertexAttribArray(0);

    ntr_glUseProgram(program);
    ntr_glBindVertexArray(VAO);
    ntr_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // Delete allocations
    ntr_glDeleteVertexArrays(1, &VAO);
    ntr_glDeleteBuffers(1, &VBO);
    ntr_glDeleteBuffers(1, &EBO);
    ntr_glDeleteProgram(program);
}

void ntr_box(i16 x, i16 y, u16 width, u16 height, color col)
{
    f32 vertices[] = {
        x,         y,          0,
        x,         y + height, 0,
        x + width, y,          0,
        x + width, y + height, 0
    };

    ntr_map_floats(8, &vertices[0], &vertices[1], &vertices[3], &vertices[4],
                      &vertices[6], &vertices[7], &vertices[9], &vertices[10]);

    u32 indices[] = {
        0,1,2,
        1,2,3
    };

    u32 program, VAO, VBO, EBO;
    ntr_create_program(&program,vertexShaderSource,ntr_frag_shader_src(col));

    ntr_glGenVertexArrays(1, &VAO);
    ntr_glGenBuffers(1, &VBO);
    ntr_glGenBuffers(1, &EBO);

    ntr_glBindVertexArray(VAO);

    ntr_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ntr_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    ntr_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    ntr_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);

    ntr_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3 * sizeof(f32), (void *)0);
    ntr_glEnableVertexAttribArray(0);

    ntr_glUseProgram(program);
    ntr_glBindVertexArray(VAO);
    ntr_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Delete allocations
    ntr_glDeleteVertexArrays(1, &VAO);
    ntr_glDeleteBuffers(1, &VBO);
    ntr_glDeleteBuffers(1, &EBO);
    ntr_glDeleteProgram(program);
}

void ntr_box_box(box_t box, color col){
    ntr_box(box.x, box.y, box.w, box.h, col);
}

void ntr_poly(vec2f32 ctr, u8 sides, f32 radius, f32 rot, color col)
{
    float vertices[3 * (sides+1)];
    memset(vertices, 0, sizeof(vertices));
    vertices[0] = ctr.x;
    vertices[1] = ctr.y;

    uint32_t indices[3 * sides];
    memset(indices, 0, sizeof(indices));

    ntr_map_floats(2, &vertices[0], &vertices[1]);

    for(int i = 3; i < 3 *(sides+1); i+=3){
            vertices[i]   = ctr.x + radius * cos( ( (360/sides) * (i/3-1) + rot ) * PI / 180);
            vertices[i+1] = ctr.y + radius * sin( ( (360/sides) * (i/3-1) + rot) * PI / 180);      
            indices[i-3]    = i/3;              
            indices[i-1]  = ((i/3) +1);                
            ntr_map_floats(2, &vertices[i], &vertices[i+1]);
    }
    indices[3*sides-1] = 1;

    uint32_t program, VAO, VBO, EBO;
    ntr_create_program(&program,vertexShaderSource,ntr_frag_shader_src(col));

    ntr_glGenVertexArrays(1, &VAO);
    ntr_glGenBuffers(1, &VBO);
    ntr_glGenBuffers(1, &EBO);

    ntr_glBindVertexArray(VAO);

    ntr_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ntr_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    ntr_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    ntr_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);

    ntr_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3 * sizeof(float), (void *)0);
    ntr_glEnableVertexAttribArray(0);

    ntr_glUseProgram(program);
    ntr_glBindVertexArray(VAO);
    ntr_glDrawElements(GL_TRIANGLES, 3 * (sides+1), GL_UNSIGNED_INT, 0);
        // Delete allocations
    ntr_glDeleteVertexArrays(1, &VAO);
    ntr_glDeleteBuffers(1, &VBO);
    ntr_glDeleteBuffers(1, &EBO);
    ntr_glDeleteProgram(program);
}

bool ntr_collide_pt_box(vec2f32 pt, box_t box)
{
    if(pt.x >= box.x && box.x <= box.x + box.w && pt.y >= box.y && pt.y <= box.y + box.h) return true;
    else return false;
}

bool ntr_collide_pt_tri(vec2f32 pt, vec2f32 a ,vec2f32 b ,vec2f32 c)
{
    uint8_t count = 0;
    vec4f32 ln[3]= {{a.x, a.y, b.x, b.y}, {b.x ,b.y ,c.x ,c.y}, {c.x, c.y, a.x, a.y}};

    for(int i = 0; i < 3; i++){
        if(((pt.y<ln[i].y) != (pt.y<ln[i].w)) && 
             pt.x < ln[i].x + (pt.y - ln[i].y)/(ln[i].w - ln[i].y) * (ln[i].z - ln[i].x)) count++;
    }
    if(count % 2 ==1) return true;
    else return false;
}

bool ntr_collide_pt_poly(vec2f32 pt, vec2f32 ctr, u16 sides, f32 radius)
{
    u8 count = 0;
    vec4f32 ln[sides];

    for(u16 i = 0; i < sides; i++){                                                    
            ln[i].x  = ctr.x + radius * cos( ( (360/sides) * (i % sides)     ) * PI / 180);
            ln[i].y  = ctr.y + radius * sin( ( (360/sides) * (i % sides)     ) * PI / 180);
            ln[i].z  = ctr.x + radius * cos( ( (360/sides) * ((i+1) % sides) ) * PI / 180);
            ln[i].w  = ctr.y + radius * sin( ( (360/sides) * ((i+1) % sides) ) * PI / 180);
            if(((pt.y<ln[i].y) != (pt.y<ln[i].w)) && 
            pt.x < ln[i].x + (pt.y - ln[i].y)/(ln[i].w - ln[i].y) * (ln[i].z - ln[i].x)) count++;
    }
    if(count % 2 ==1) return true;
    else return false;
}

bool ntr_collide_pt_line(vec2f32 pt, vec2f32 start, vec2f32 end, f32 thick)
{
    f32 dx, dy, m, deg;
    vec2f32 s2, e2;

    dx = end.x - start.x;
    dy = end.y - start.y;
    if(dx) {
        m = dy / dx;
        deg = atanf(m) * 180 / PI;
        if(m < 0) deg = -deg;            // Slope nwgaive (to us it will look postive
        if(m){
            s2.x = start.x + thick * cosf(fmodf(deg + 270.0f, 360.0f));
            s2.y = start.y - thick * sinf(fmodf(deg + 270.0f, 360.0f));

            e2.x = end.x + thick * cosf(fmodf(deg + 270.0f, 360.0f));
            e2.y = end.y - thick * sinf(fmodf(deg + 270.0f, 360.0f));
        }
        else{
            s2 = (vec2f32){start.x, start.y - thick};
            e2 = (vec2f32){end.x, end.y - thick};
        }
    }
    else{
            s2 = (vec2f32){start.x + thick, start.y};
            e2 = (vec2f32){end.x + thick, end.y};
    }
    if( ntr_collide_pt_tri(pt, start, s2, e2)  ) return true;
    if( ntr_collide_pt_tri(pt, end, e2, start) ) return true;
    return false;
}