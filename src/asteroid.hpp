// vim: ts=4 sw=4 sts=4 et
#pragma once

#include "entity.hpp"


class Asteroid : public Entity {
public:

    Asteroid(const glm::vec2& pos, int size = 3, const glm::vec2* dir = nullptr);
    void update() override;
    void collision(Entity& other) override;
    void draw(const ALLEGRO_TRANSFORM& transform) override;

private:
    float                     m_vang;
    int                       m_size;
    int                       m_health;
    int                       m_hit_delay = 0;
};
