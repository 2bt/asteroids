// vim: ts=4 sw=4 sts=4 et
#include "asteroid.hpp"
#include "world.hpp"

#include "bullet.hpp"
#include "ship.hpp"


Asteroid::Asteroid(const glm::vec2& pos, int size, const glm::vec2* dir) {
    m_pos    = pos;
    m_size   = size;
    m_health = m_size;
    m_ang    = world.random_float(0, 2 * ALLEGRO_PI);

    if (dir) m_vel = *dir;
    else     m_vel = { std::sin(m_ang), std::cos(m_ang) };

    float r  = m_health * 10;
    m_pos   += m_vel * r * 0.3f;
    float f  = std::pow(0.8, world.level_nr()) * 0.1 + 1;
    m_vel   *= world.random_float(0.25, 0.5) * (4 - m_size) * f;
    m_vang   = world.random_float(-1, 1) * 0.01 * (4 - m_size) * f;

    // unique mesh
    m_vertices.emplace_back();
    const int N = 12;
    for (int j = 0; j < N; ++j) {
        float r2 = world.random_float(0.7, 1.3) * r;
        float ang = (j + world.random_float(0, 0.9)) / N * 2 * ALLEGRO_PI;
        m_radius = std::max(m_radius, r2);
        glm::vec2 v = { std::sin(ang), std::cos(ang) };
        m_vertices.emplace_back(v * r2);
        m_triangles.emplace_back(0, j + 1, (j + 1) % N + 1);
    }

    m_collision_category = CC_ASTEROID;
    m_collision_mask     = CC_BULLET | CC_SHIP;
}


void Asteroid::collision(Entity& other) {
//    if (dynamic_cast<Asteroid*>(&other)) {
//        glm::vec2 n = glm::normalize(other.pos() - m_pos);
//        m_vel -= 2.0f * glm::dot(n, m_vel) * n;
//        return;
//    }

    m_hit_delay = 10;
    if (--m_health <= 0) {
        die();

        int score = m_size == 3 ? 20 : m_size == 2 ? 50 : 100;
        if (Bullet* b = dynamic_cast<Bullet*>(&other)) b->player().inc_score(score);
        if (Ship*   s = dynamic_cast<Ship*  >(&other)) s->player().inc_score(score);

        world.spawn_explosion(m_pos, m_radius);
        if (m_size > 1) {
            Entity& a1 = world.spawn(std::make_unique<Asteroid>(m_pos, m_size - 1));
            Entity& a2 = world.spawn(std::make_unique<Asteroid>(m_pos, m_size - 1));
            glm::vec2 dir = -glm::normalize(a1.vel() + a2.vel());
            world.spawn(std::make_unique<Asteroid>(m_pos, m_size - 1, &dir));
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
    int c = std::min(255, std::min(255, 170 + m_hit_delay * 15));
    al_use_transform(&transform);
    al_draw_polygon((float*) &m_transformed_vertices[1], m_triangles.size(),
                    ALLEGRO_LINE_JOIN_ROUND, al_map_rgb(c, c, c), 1, 0);

}

