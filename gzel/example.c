#include <gzel/gzel.h>

u16 screen_width  = 1920;
u16 screen_height = 1080;

int main()
{   
    gzel_set_window(screen_width, screen_height, "My Program!");
    gzel_link_gl_platfrom();

    while (!gzel_window_should_close())
    {   
        gzel_egl_swap_buffers();
        gzel_poll_events();
        gzel_clear_backgorund(CYAN);
        gzel_draw_text("Hello world", 100, 100, 14, BLACK);
        //
        gzel_reset_events();
    }
    return 0;
}