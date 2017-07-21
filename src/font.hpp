// vim: ts=4 sw=4 sts=4 et
#pragma once

#include <array>
#include <glm/glm.hpp>


class Font {
public:
    enum class Align { LEFT, CENTER };

    Font();
    void printf(const glm::vec2& pos, const char* format, ...) const;
    void print(const glm::vec2& pos, const char* text) const;

    void set_line_width(float w) { m_line_width = w; };
    void set_size(float s) { m_size = s; };
    void set_align(Align a) { m_align = a; };


private:
    std::array<std::array<int, 10>, 128> m_data;
    float m_line_width = 1;
    float m_size       = 1;
    Align m_align      = Align::LEFT;
};


extern Font font;
