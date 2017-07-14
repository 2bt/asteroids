// vim: ts=4 sw=4 sts=4 et
#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>


enum {
    WIDTH  = 320 * 2,
    HEIGHT = 180 * 2,
};


inline float random_float(float a, float b) {
    return a + rand() / (float) RAND_MAX * (b - a);
}


struct CollisionInfo {
    glm::vec2 pos;
    glm::vec2 normal;
    float     distance;
};


class Entity {
public:

    enum {
        CC_ASTEROID = 0x0001,
        CC_BULLET   = 0x0002,
        CC_SHIP     = 0x0004,
    };

    using Ptr = std::unique_ptr<Entity>;
    virtual void update() {}
    virtual void draw(const ALLEGRO_TRANSFORM& transform) {}
    virtual void collision(Entity& other) {}
    virtual ~Entity() {}

    bool is_alive() const { return m_is_alive; }
    const glm::vec2& pos() const { return m_pos; }
    const glm::vec2& vel() const { return m_vel; }
    float radius() const { return m_radius; }

    void die() { m_is_alive = false; }
    bool check_collision(const Entity& other) const;

    void update_transformed_vertices() {
        m_transformed_vertices.resize(m_vertices.size());
        std::copy(m_vertices.begin(), m_vertices.end(), m_transformed_vertices.begin());
        ALLEGRO_TRANSFORM t;
        al_identity_transform(&t);
        al_rotate_transform(&t, m_ang);
        al_translate_transform(&t, m_pos.x, m_pos.y);
        for (glm::vec2 & v : m_transformed_vertices) al_transform_coordinates(&t, &v.x, &v.y);
    }



protected:
    glm::vec2 m_pos;
    float     m_ang;

    glm::vec2 m_vel;


    // collision stuff
    std::vector<glm::vec2>  m_vertices;
    std::vector<glm::ivec3> m_triangles;
    std::vector<glm::vec2>  m_transformed_vertices;
    float                   m_radius             = 0;
    uint32_t                m_collision_category = 0;
    uint32_t                m_collision_mask     = 0;


    void warp_pos() {
        if (m_pos.x < 0)      m_pos.x += WIDTH;
        if (m_pos.x > WIDTH)  m_pos.x -= WIDTH;
        if (m_pos.y < 0)      m_pos.y += HEIGHT;
        if (m_pos.y > HEIGHT) m_pos.y -= HEIGHT;
    }

private:
    bool check_collision(const Entity& other, const glm::vec2& offset) const;

    bool m_is_alive = true;
};
