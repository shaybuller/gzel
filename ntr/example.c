#include <ntr/ntr.h>

u16 screen_width  = 1920;
u16 screen_height = 1080;

int main()
{   
    ntr_set_window(screen_width, screen_height, "My Program!");
    ntr_link_gl_platfrom();

    while (!ntr_window_should_close())
    {   
        ntr_egl_swap_buffers();
        ntr_poll_events();
        ntr_clear_backgorund(CYAN);
        ntr_draw_text("Hello world", 100, 100, 14, BLACK);
        //
        ntr_reset_events();
    }
    return 0;
}