#include <gzel/gzel.h>

u16 screen_width  = 500;
u16 screen_height = 500;

int main()
{   
    gzel_set_window(screen_width, screen_height, "Hello Triangle");
    gzel_link_gl_platfrom();

    while (!gzel_window_should_close())
    {   
        // Begin Draw
        gzel_start_cycle();
        // Hello Triangle
        gzel_clear_backgorund(CYAN);
        gzel_tri((vec2f32){0,500}, (vec2f32){250,0}, (vec2f32){500,500}, RED);
        // End Draw
        gzel_end_cycle();
    }
    return 0;
}