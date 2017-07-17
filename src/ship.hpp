// vim: ts=4 sw=4 sts=4 et
#pragma once

#include "entity.hpp"
#include "player.hpp"


class Ship : public Entity {
public:
    Ship(Player& player, bool invincible);
    void update() override;
    void collision(Entity& other) override;
    void draw(const ALLEGRO_TRANSFORM& transform) override;
    Player& player() { return m_player; }

private:
    int       m_thrust_counter = 0;
    int       m_shoot_delay    = 0;
    int       m_noclip_counter;
    Player&   m_player;
};
