// vim: ts=4 sw=4 sts=4 et
#pragma once

#include <array>
#include <glm/glm.hpp>


class Font {
public:
    Font();
    void printf(const glm::vec2& pos, const char* format, ...) const;
    void print(const glm::vec2& pos, const char* text) const;

    void set_line_width(float w) { m_line_width = w; };
    void set_size(float s) { m_size = s; };


private:
    std::array<std::array<int, 10>, 128> m_data;
    float m_size = 1;
    float m_line_width = 1;
};


extern Font font;
