// vim: ts=4 sw=4 sts=4 et
#pragma once

#include <array>

class Audio {
public:

    enum {
        MIX_RATE    = 48000,
        BUFFER_LEN  = 512,
    };

    void mix(float* buffer);

    void sound(int type, float panning);

private:
    void mix_frame(float* frame);

    struct Voice {
        enum State { OFF, HOLD, ATTACK };
        State state;
        float pan;
        float level;
        float pos;
        float pitch;
        int   len;
    };

    std::array<Voice, 8> m_voices;
    int                  m_voice_index;
};

extern Audio audio;
