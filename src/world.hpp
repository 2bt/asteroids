// vim: ts=4 sw=4 sts=4 et
#pragma once

#include <vector>
#include "entity.hpp"
#include "player.hpp"


class World {
public:
    void init();
    void update();
    void draw();

    Entity& spawn(Entity::Ptr&& e);
    void spawn_explosion(const glm::vec2& pos, float r);

private:
    std::vector<Entity::Ptr> m_entities;
    std::vector<Entity::Ptr> m_new_entities;
    Player                   m_player;

    bool                     m_level_done;
};


extern World world;
