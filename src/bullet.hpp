// vim: ts=4 sw=4 sts=4 et
#pragma once

#include "entity.hpp"


class Bullet : public Entity {
public:
    Bullet(const glm::vec2& pos, float ang) {
        m_ang = ang;
        m_vel.x = -std::sin(m_ang) * 5;
        m_vel.y =  std::cos(m_ang) * 5;
        m_pos = pos + m_vel * 2.0f;

        m_radius = 1.5;
        m_triangles = { { 0, 1, 2 } };
        m_vertices = { { 0, 1 }, { 1, -1 }, { -1, -1 } };
        m_collision_category = CC_BULLET;
        m_collision_mask     = CC_ASTEROID;
    }

    void update() override {
        if (--m_ttl <= 0) die();
        m_pos += m_vel;
        warp_pos();
    }

    void collision(Entity& other) override {
        die();
    }

    void draw(const ALLEGRO_TRANSFORM& transform) override {
        ALLEGRO_TRANSFORM t;
        al_identity_transform(&t);
        al_rotate_transform(&t, m_ang);
        al_translate_transform(&t, m_pos.x, m_pos.y);
        al_compose_transform(&t, &transform);
        al_use_transform(&t);
        float s = std::min(1.0, m_ttl * 0.2);
        al_draw_filled_ellipse(0, 0, s, s * 3, al_map_rgb(255, 255, 255));
//        al_use_transform(&transform);
//        al_draw_polygon((float*) &m_transformed_vertices[0], 3,
//                ALLEGRO_LINE_JOIN_ROUND, al_map_rgb(255, 100, 100), 1, 0);
    }

private:
    int m_ttl = 50;
};
