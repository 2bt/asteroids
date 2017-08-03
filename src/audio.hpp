// vim: ts=4 sw=4 sts=4 et
#pragma once

#include <array>

enum SoundType {
    ST_SMALL_BANG,
    ST_MEDIUM_BANG,
    ST_LARGE_BANG,
    ST_FIRE,
    ST_THRUST,
    ST_HIT,
};


class Audio {
public:
    enum {
        MIX_RATE    = 48000,
        BUFFER_LEN  = 512,
    };

    void sound(SoundType type, float panning);
    void mix(float* buffer);

private:
    void mix_frame(float* frame);

    struct Voice {
        enum State { OFF, HOLD, ATTACK };
        enum Wave { SAW, TRI, NOISE };

        float     decay;
        float     pitch;
        float     sweep;
        float     vol;
        int       wave;

        State     state;
        float     pan;
        float     level;
        float     pos;
        float     noise;
        int       len;
    };

    std::array<Voice, 8> m_voices;
    int                  m_voice_index = 0;
};

extern Audio audio;
