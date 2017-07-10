// vim: ts=4 sw=4 sts=4 et
#pragma once

#include "entity.hpp"


class Ship : public Entity {
public:

    Ship();
    void update() override;
    void collision(Entity& other) override;
    void draw(const ALLEGRO_TRANSFORM& transform) override;

private:
    int       m_thrust      = 0;
    int       m_shoot_delay = 0;
};
