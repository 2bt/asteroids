// vim: ts=4 sw=4 sts=4 et
#pragma once

#include "entity.hpp"
#include "player.hpp"


class Bullet : public Entity {
public:
    Bullet(Player& player, const glm::vec2& pos, float ang);
    void update() override;
    void collision(Entity& other) override;
    void draw(const ALLEGRO_TRANSFORM& transform) override;
    Player& player() { return m_player; }

private:
    Player& m_player;
    int     m_ttl = 50;
};
