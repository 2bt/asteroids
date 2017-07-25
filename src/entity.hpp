// vim: ts=4 sw=4 sts=4 et
#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>


enum {
    WIDTH  = 320 * 2,
    HEIGHT = 180 * 2,
};


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
    virtual bool needs_killing() const { return false; }


    bool             is_alive()           const { return m_is_alive; }
    const glm::vec2& pos()                const { return m_pos; }
    const glm::vec2& vel()                const { return m_vel; }
    float            radius()             const { return m_radius; }
    int              collision_mask()     const { return m_collision_mask; }
    int              collision_category() const { return m_collision_category; }

    void die() { m_is_alive = false; }
    bool check_collision(const Entity& other, const glm::vec2& offset) const;
    void update_transformed_vertices();


protected:
    glm::vec2 m_pos;
    float     m_ang;

    glm::vec2 m_vel;


    // collision stuff
    std::vector<glm::vec2>  m_vertices;
    std::vector<glm::ivec3> m_triangles;
    std::vector<glm::vec2>  m_transformed_vertices;
    float                   m_radius             = 0;
    int                     m_collision_category = 0;
    int                     m_collision_mask     = 0;


    void warp_pos() {
        if (m_pos.x < 0)      m_pos.x += WIDTH;
        if (m_pos.x > WIDTH)  m_pos.x -= WIDTH;
        if (m_pos.y < 0)      m_pos.y += HEIGHT;
        if (m_pos.y > HEIGHT) m_pos.y -= HEIGHT;
    }

private:
    bool m_is_alive = true;
};
