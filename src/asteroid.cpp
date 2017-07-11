// vim: ts=4 sw=4 sts=4 et
#include "asteroid.hpp"
#include "world.hpp"

#include "bullet.hpp"


Asteroid::Asteroid(const glm::vec2& pos, int size, const glm::vec2* dir) {
    m_pos    = pos;
    m_size   = size;
    m_health = m_size;
    m_ang    = random_float(0, 2 * ALLEGRO_PI);
    m_vang   = random_float(-1, 1) * 0.01 * (4 - m_size);

    if (dir) m_vel = *dir;
    else     m_vel = { std::sin(m_ang), std::cos(m_ang) };

    float r  = m_health * 10;
    m_pos   += m_vel * r * 0.3f;
    m_vel   *= random_float(0.25, 0.5) * (4 - m_size);

    // unique mesh
    m_vertices.emplace_back();
    const int N = 12;
    for (int j = 0; j < N; ++j) {
        float r2 = random_float(0.7, 1.3) * r;
        float ang = (j + random_float(0, 0.9)) / N * 2 * ALLEGRO_PI;
        glm::vec2 v;
        v.x = std::sin(ang) * r2;
        v.y = std::cos(ang) * r2;

        m_radius = std::max(m_radius, r2);
        m_vertices.emplace_back(v.x, v.y);
        m_triangles.emplace_back(0, j, (j + 1) % N);
    }

    m_collision_category = CC_ASTEROID;
    m_collision_mask     = CC_BULLET | CC_SHIP;
}


void Asteroid::collision(Entity& other) {
    if (Bullet* b = dynamic_cast<Bullet*>(&other)) {
        b->die();
        m_hit_delay = 5;
        if (--m_health <= 0) {
            die();
            world.spawn_explosion(m_pos, m_radius);
            if (m_size > 1) {
                Entity& a1 = world.spawn(std::make_unique<Asteroid>(m_pos, m_size - 1));
                Entity& a2 = world.spawn(std::make_unique<Asteroid>(m_pos, m_size - 1));

                glm::vec2 dir = -glm::normalize(a1.vel() + a2.vel());
                world.spawn(std::make_unique<Asteroid>(m_pos, m_size - 1, &dir));
            }
        }
    }
}


void Asteroid::update() {
    if (m_hit_delay > 0) --m_hit_delay;
    m_ang += m_vang;
    m_pos += m_vel;
    warp_pos();
}


void Asteroid::draw(const ALLEGRO_TRANSFORM& transform) {
    int c = std::min(255, 170 + m_hit_delay * 15);
    al_use_transform(&transform);
    al_draw_polygon((float*) &m_transformed_vertices[1], m_triangles.size(),
                    ALLEGRO_LINE_JOIN_ROUND, al_map_rgb(c, c, c), 1, 0);

}

