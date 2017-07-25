// vim: ts=4 sw=4 sts=4 et
#pragma once

#include <vector>
#include <random>
#include "entity.hpp"
#include "player.hpp"


class World {
public:
    void init();
    void update();
    void draw();

    Entity& spawn(Entity::Ptr&& e);
    void spawn_explosion(const glm::vec2& pos, float r);

    int level_nr() const { return m_level_nr; }
    bool running() const { return m_running; }

    float random_float(float a, float b) {
        return std::uniform_real_distribution<float>(a, b)(m_random);
    }


private:
    void init_level(int nr);

    std::vector<Entity::Ptr>   m_entities;
    std::vector<Entity::Ptr>   m_new_entities;
    Player                     m_player;

    int                        m_level_nr;
    bool                       m_level_done;
    int                        m_done_counter;
    bool                       m_running;
    std::default_random_engine m_random;
};


extern World world;
