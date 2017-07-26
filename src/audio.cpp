// vim: ts=4 sw=4 sts=4 et
#include "audio.hpp"
#include <cmath>


void Audio::sound(int type, float panning) {
	Voice& v = m_voices[m_voice_index];
	m_voice_index = (m_voice_index + 1) % m_voices.size();

	v.state = Voice::ATTACK;
	v.level = 0;
	v.pan   = panning - 0.5;
	v.len   = 1000;

}

void Audio::mix_frame(float* frame) {

//	static double t = 0;
//	frame[0] = frame[1] = std::sin(t * 2 * M_PI) * 0.1;
//	t += 440.0 / MIX_RATE;

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
			v.level *= 0.999;
			break;
		default:
			v.level *= 0.99;
		}

		v.pos += (50.0 + v.len * 5) / MIX_RATE;
		v.pos -= (int) v.pos;
		float amp = v.pos * 2 - 1;
		amp *= v.level * 0.3;

		frame[0] += amp * std::sqrt(0.5 - v.pan);
		frame[1] += amp * std::sqrt(0.5 + v.pan);
//		frame[0] += amp;
//		frame[1] += amp;
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
