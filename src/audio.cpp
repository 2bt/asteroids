// vim: ts=4 sw=4 sts=4 et
#include "audio.hpp"
#include <cmath>


void Audio::sound(SoundType type, float panning) {
    Voice& v = m_voices[m_voice_index];
    m_voice_index = (m_voice_index + 1) % m_voices.size();

    v.state = Voice::ATTACK;
    v.level = 0;
    v.pan   = panning - 0.5;
    v.type  = type;

    switch (type) {
    case ST_FIRE:
        v.wave  = Voice::SAW;
        v.len   = 9000;
        v.decay = 0.9999;
        v.pitch = 14;
        v.sweep = -0.0015;
        v.vol   = 0.5;
        break;
    case ST_SMALL_BANG:
    case ST_MEDIUM_BANG:
    case ST_LARGE_BANG:
        v.wave  = Voice::NOISE;
        v.len   = 80000;
        v.decay = 0.99994;
        v.pitch = 16 - (type - ST_SMALL_BANG) * 4;
        v.sweep = 0;
        v.vol   = 1;
        break;
    case ST_THRUST:
        v.wave  = Voice::TRI;
        v.len   = 3000;
        v.decay = 0.9999;
        v.pitch = -30;
        v.sweep = 0.0005;
        v.vol   = 0.5;
        break;
    default:
        break;
    }

}

void Audio::mix_frame(float* frame) {
    for (Voice& v : m_voices) {
        if (v.state == Voice::OFF && v.level < 0.001) continue;
        if (v.len > 0 && --v.len == 0) v.state = Voice::OFF;
        switch (v.state) {
        case Voice::ATTACK:
            v.level += 0.01;
            if (v.level >= 1) {
                v.level = 1;
                v.state = Voice::HOLD;
            }
            break;
        case Voice::HOLD:
            v.level *= v.decay;
            break;
        default:
            v.level *= 0.999;
        }

        v.pitch += v.sweep;
        float speed = std::exp2(v.pitch / 12) * 440 / MIX_RATE;
        v.pos += speed;
        v.pos -= (int) v.pos;

        float amp;
        if      (v.wave == Voice::SAW) amp = v.pos * 2 - 1;
        if      (v.wave == Voice::TRI) amp = v.pos < 0.5 ? 4 * v.pos - 1 : 3 - 4 * v.pos;
        else if (v.wave == Voice::NOISE) {
            if (v.pos < speed) v.noise = rand() * 2.0 / RAND_MAX - 1;
            amp = v.noise;
        }

        amp *= v.level * v.vol * 0.5;
        frame[0] += amp * std::sqrt(0.5 - v.pan);
        frame[1] += amp * std::sqrt(0.5 + v.pan);
    }
}


void Audio::mix(float* buffer) {
    for (int i = 0; i < BUFFER_LEN; ++i) {
        buffer[0] = 0;
        buffer[1] = 0;
        mix_frame(buffer);
        buffer[0] = std::max(-1.0f, std::min(1.0f, buffer[0]));
        buffer[1] = std::max(-1.0f, std::min(1.0f, buffer[1]));
        buffer += 2;
    }
}

Audio audio;
