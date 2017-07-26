// vim: ts=4 sw=4 sts=4 et
#include "world.hpp"
#include "audio.hpp"
#include <allegro5/allegro_audio.h>


enum { FPS = 60 };


void resize(int width, int height) {
    al_reset_clipping_rectangle();
    al_clear_to_color(al_map_rgb(0, 0, 0));
    ALLEGRO_TRANSFORM t;
    al_identity_transform(&t);
    if (width * HEIGHT < height * WIDTH) {
        float w = width  / (float) WIDTH;
        al_scale_transform(&t, w, w);
        al_translate_transform(&t, 0, (height - w * HEIGHT) * 0.5);
        int h = width * HEIGHT / WIDTH;
        al_set_clipping_rectangle(0, (height - h) / 2, width, h);
    }
    else {
        float h = height  / (float) HEIGHT;
        al_scale_transform(&t, h, h);
        al_translate_transform(&t, (width - h * WIDTH) * 0.5, 0);
        int w = height * WIDTH / HEIGHT;
        al_set_clipping_rectangle((width - w) / 2, 0, w, height);
    }
    al_use_transform(&t);
}


int main(int argc, char** argv) {
    al_init();

    al_init_primitives_addon();
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_SUGGEST);
    ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
    al_hide_mouse_cursor(display);

    resize(al_get_display_width(display), al_get_display_height(display));


    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    al_install_audio();
    al_reserve_samples(0);
    ALLEGRO_AUDIO_STREAM* stream = al_create_audio_stream(4, Audio::BUFFER_LEN, Audio::MIX_RATE,
                                                          ALLEGRO_AUDIO_DEPTH_FLOAT32,
                                                          ALLEGRO_CHANNEL_CONF_2);

    ALLEGRO_EVENT_QUEUE * queue = al_create_event_queue();
    al_install_keyboard();
    al_install_joystick();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_audio_stream_event_source(stream));

    al_start_timer(timer);
    al_attach_audio_stream_to_mixer(stream, al_get_default_mixer());

    world.init();
    bool redraw = false;
    while (world.running()) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) break;
        }
        else if (event.type == ALLEGRO_EVENT_AUDIO_STREAM_FRAGMENT) {
            ALLEGRO_AUDIO_STREAM *stream = (ALLEGRO_AUDIO_STREAM *) event.any.source;
            void* buf = al_get_audio_stream_fragment(stream);
            if (buf) {
                audio.mix((float*) buf);
                al_set_audio_stream_fragment(stream, buf);
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
            redraw = true;
            resize(event.display.width, event.display.height);
            al_acknowledge_resize(display);
        }
        else if (event.type == ALLEGRO_EVENT_JOYSTICK_CONFIGURATION) {
            al_reconfigure_joysticks();
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
            world.update();
        }
        if (redraw && al_event_queue_is_empty(queue)) {
            redraw = false;
            world.draw();
        }

    }

    al_destroy_audio_stream(stream);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    al_destroy_display(display);
    return 0;
}
