#include "ntr.h"
#include <stdlib.h>

xcb_generic_event_t *event;
xcb_window_t window;
xcb_connection_t *connection;
xcb_intern_atom_reply_t *delete_reply;

bool window_should_close = false;
i16 MOUSE_PRESS_SIGN, MOUSE_HOLD_SIGN, MOUSE_RELEASE_SIGN;
i16 KEY_PRESS_SIGN,   KEY_HOLD_SIGN,   KEY_RELEASE_SIGN;
vec2f32 mpos, press_pos;

void ntr_poll_events()
{
    bool finish = false;

    for (u8 i = 0; i < 64; i++) {
        if ((event = xcb_poll_for_event(connection))) {
            switch (event->response_type & ~0x80) {

            case XCB_BUTTON_PRESS: {
                xcb_button_press_event_t *bp = (xcb_button_press_event_t *)event;
                press_pos       = (vec2f32){bp->event_x, bp->event_y};
                if(!MOUSE_HOLD_SIGN) MOUSE_PRESS_SIGN = bp->detail;
                MOUSE_HOLD_SIGN  = bp->detail;
                break;
            }

            case XCB_BUTTON_RELEASE: {
                xcb_button_release_event_t *br = (xcb_button_release_event_t *)event;
                MOUSE_RELEASE_SIGN = br->detail;
                MOUSE_HOLD_SIGN = 0;
                break;
            }

            case XCB_KEY_PRESS: {
                xcb_key_press_event_t *kp = (xcb_key_press_event_t *)event;
                if(!KEY_HOLD_SIGN) KEY_PRESS_SIGN = kp->detail;
                KEY_HOLD_SIGN = kp->detail;
                break;
            }

            case XCB_KEY_RELEASE: {
                xcb_key_release_event_t *kr = (xcb_key_release_event_t *)event;
                KEY_RELEASE_SIGN = kr->detail;
                KEY_HOLD_SIGN = 0;
                break;
            }

            case XCB_MOTION_NOTIFY: {
                xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)event;
                mpos = (vec2f32){motion->event_x, motion->event_y};
                break;
            }

            case XCB_CLIENT_MESSAGE: {
                xcb_client_message_event_t *client =(xcb_client_message_event_t *)event;
                if (client->data.data32[0] == (*delete_reply).atom) {
                    window_should_close = true;
                    finish = true;
                }
                break;
            }

            default:
                break;
            }
        }

        fflush(stdout);
        free(event);
        event = NULL;

        if (finish) {
            ntr_disconnect_gl();
            free(delete_reply);
            delete_reply = NULL;
            free(event);
            event = NULL;
            xcb_disconnect(connection);
            exit(0);
        }
    }
}

bool ntr_window_xcb_should_close(){
    return window_should_close;
}

void ntr_reset_events()
{
    MOUSE_PRESS_SIGN   = 0;
    MOUSE_RELEASE_SIGN = 0;
    KEY_PRESS_SIGN     = 0;
    KEY_RELEASE_SIGN   = 0;
    press_pos = (vec2f32){0};
}

bool ntr_is_press(i16 key)
{
    return MOUSE_PRESS_SIGN == key || KEY_PRESS_SIGN == key;
}

bool ntr_is_hold(i16 key)
{
    return MOUSE_HOLD_SIGN == key || KEY_HOLD_SIGN == key;
}

bool ntr_is_release(i16 key)
{
    return MOUSE_RELEASE_SIGN == key || KEY_RELEASE_SIGN == key;
}

void ntr_xcb_set_window(u16 width, u16 height, const char *title)
{
    connection = xcb_connect(NULL, NULL);

    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
    window = xcb_generate_id(connection);

    uint32_t mask = XCB_CW_EVENT_MASK;
    uint32_t value =
        XCB_EVENT_MASK_EXPOSURE |
        XCB_EVENT_MASK_KEY_PRESS |
        XCB_EVENT_MASK_KEY_RELEASE |
        XCB_EVENT_MASK_BUTTON_PRESS |
        XCB_EVENT_MASK_BUTTON_RELEASE |
        XCB_EVENT_MASK_POINTER_MOTION;

    xcb_create_window(
        connection,
        XCB_COPY_FROM_PARENT,
        window,
        screen->root,
        0, 0,
        width, height,
        10,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        screen->root_visual,
        mask, &value
    );

    xcb_change_property(
        connection,
        XCB_PROP_MODE_REPLACE,
        window,
        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,
        8,
        strlen(title),
        title
    );

    xcb_intern_atom_cookie_t protocols_cookie = xcb_intern_atom(connection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_cookie_t delete_cookie    = xcb_intern_atom(connection, 0, 16, "WM_DELETE_WINDOW");
    xcb_intern_atom_reply_t *protocols_reply  = xcb_intern_atom_reply(connection, protocols_cookie, NULL);
    delete_reply = xcb_intern_atom_reply(connection, delete_cookie, NULL);

    xcb_change_property(
        connection,
        XCB_PROP_MODE_REPLACE,
        window,
        (*protocols_reply).atom,
        4, 32, 1,
        &(*delete_reply).atom
    );

    free(protocols_reply);
    protocols_reply = NULL;
    
    xcb_xkb_use_extension(connection, XCB_XKB_MAJOR_VERSION, XCB_XKB_MINOR_VERSION);

    xcb_xkb_per_client_flags(
        connection,
        XCB_XKB_ID_USE_CORE_KBD,
        XCB_XKB_PER_CLIENT_FLAG_DETECTABLE_AUTO_REPEAT,
        XCB_XKB_PER_CLIENT_FLAG_DETECTABLE_AUTO_REPEAT,
        0,
        0,
        0
    );

    xcb_map_window(connection, window);
    xcb_flush(connection);
}

vec2f32 ntr_get_window_size()
{
    xcb_get_geometry_cookie_t geomCookie = xcb_get_geometry(connection, window);
    xcb_get_geometry_reply_t *geom       = xcb_get_geometry_reply(connection, geomCookie, NULL);
    vec2f32 size = (vec2f32){geom->width, geom->height};
    free(geom);
    geom = NULL;
    return size;
}
