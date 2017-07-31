// vim: ts=4 sw=4 sts=4 et
#pragma once

#include "entity.hpp"


class Particle : public Entity {
public:
    Particle(const glm::vec2& pos, const glm::vec2& vel, int ttl) {
        m_pos = pos;
        m_vel = vel;
        m_ttl = ttl;
    }

    void update() override {
        if (--m_ttl <= 0) die();
        m_pos += m_vel;
        warp_pos();
        m_vel *= 0.97;
    }

    void draw(const ALLEGRO_TRANSFORM& transform) override {
        al_use_transform(&transform);
        al_draw_filled_circle(m_pos.x, m_pos.y, std::min(1.5, m_ttl * 0.1), al_map_rgb(200, 200, 200));
    }

private:
    int m_ttl;
};
