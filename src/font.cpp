// vim: ts=4 sw=4 sts=4 et
#include "font.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <cstdarg>

Font::Font() {
    m_data = {};
    m_data['0'] = { 0x0040, 0x4046, 0x4606, 0x0600 /*, 0x0046*/ };
    m_data['1'] = { 0x2026 };
    m_data['2'] = { 0x0040, 0x4043, 0x4303, 0x0306, 0x0646 };
    m_data['3'] = { 0x0040, 0x0343, 0x0646, 0x4046 };
    m_data['4'] = { 0x0003, 0x0343, 0x4046 };
    m_data['5'] = { 0x0040, 0x0343, 0x4606, 0x0003, 0x4346 };
    m_data['6'] = { 0x0006, 0x0646, 0x4643, 0x4303 };
    m_data['7'] = { 0x0040, 0x4046 };
    m_data['8'] = { 0x0040, 0x4046, 0x0006, 0x0646, 0x0343 };
    m_data['9'] = { 0x0040, 0x4046, 0x0003, 0x0343 };

    m_data['a'] = { 0x0220, 0x2042, 0x4246, 0x0206, 0x0444 }; // a
    m_data['b'] = { 0x0030, 0x3041, 0x4142, 0x4233, 0x3344, 0x4445, 0x4536, 0x3606, 0x0006, 0x0333 };
    m_data['c'] = { 0x0040, 0x0006, 0x0646 };
    m_data['d'] = { 0x0020, 0x2042, 0x4244, 0x4426, 0x2606, 0x0006 };
    m_data['e'] = { 0x0040, 0x0333, 0x0646, 0x0006 };
    m_data['f'] = { 0x0040, 0x0333, 0x0006 };
    m_data['g'] = { 0x0040, 0x4042, 0x0006, 0x0646, 0x4644, 0x4424 };
    m_data['h'] = { 0x0006, 0x4046, 0x0343 };
    m_data['i'] = { 0x0040, 0x0646, 0x2026 };
    m_data['j'] = { 0x4046, 0x4626, 0x2604 };
    m_data['k'] = { 0x0006, 0x0340, 0x0346 };
    m_data['l'] = { 0x0006, 0x0646 };
    m_data['m'] = { 0x0006, 0x4046, 0x0022, 0x2240 };
    m_data['n'] = { 0x0006, 0x4046, 0x0046 };
    m_data['o'] = { 0x0040, 0x4046, 0x4606, 0x0600 };
    m_data['p'] = { 0x0040, 0x4043, 0x4303, 0x0600 };
    m_data['q'] = { 0x0040, 0x4044, 0x4426, 0x2606, 0x0006, 0x2446 };
    m_data['r'] = { 0x0006, 0x0040, 0x4043, 0x4303, 0x0346 };
    m_data['s'] = { 0x0040, 0x0343, 0x4606, 0x0003, 0x4346 };
    m_data['t'] = { 0x0040, 0x2026 };
    m_data['u'] = { 0x4046, 0x4606, 0x0600 };
    m_data['v'] = { 0x0026, 0x2640 };
    m_data['w'] = { 0x0006, 0x0624, 0x2446, 0x4046 };
    m_data['x'] = { 0x0046, 0x4006 };
    m_data['y'] = { 0x0022, 0x2240, 0x2226 };
    m_data['z'] = { 0x0040, 0x4006, 0x0646 };

    m_data['.'] = { 0x2526 };
    m_data[':'] = { 0x2122, 0x2425 };
}


inline int get_nibble(int d, int i) { return (d >> i * 4) & 0xf; }


void Font::printf(const glm::vec2& pos, const char* format, ...) const {
	char text[256];
	va_list args;
	va_start(args, format);
	vsnprintf(text, 256, format, args);
	va_end(args);
    print(pos, text);
}


void Font::print(const glm::vec2& pos, const char* text) const {
    glm::vec2 p = pos;
    while (char c = *text++) {
        if (c >= 0 && c < 128) {
            for (int d : m_data[c]) {
                if (d == 0) break;
                float vertices[4] = {
                    p.x + get_nibble(d, 3) * m_size,
                    p.y + get_nibble(d, 2) * m_size,
                    p.x + get_nibble(d, 1) * m_size,
                    p.y + get_nibble(d, 0) * m_size,
                };
                al_draw_polyline(vertices, 8, 2, 0, ALLEGRO_LINE_CAP_ROUND,
                                 al_map_rgb(255, 255, 255), m_line_width, 0);
            }
        }
        p.x += 6 * m_size;
    }
}


Font font;
