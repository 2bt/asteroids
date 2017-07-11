// vim: ts=4 sw=4 sts=4 et
#include "ship.hpp"
#include "bullet.hpp"
#include "world.hpp"
#include "asteroid.hpp"


Ship::Ship() {
    m_ang                = ALLEGRO_PI;
    m_radius             = glm::length(glm::vec2(5, 5));
    m_collision_category = CC_SHIP;
    m_collision_mask     = CC_ASTEROID;

    m_vertices = {
        {  0, -3 },
        {  5, -5 },
        {  0,  6 },
        { -5, -5 },
    };
    m_triangles = {
        { 0, 1, 2 },
        { 0, 2, 3 },
    };
}


void Ship::update() {
    static ALLEGRO_KEYBOARD_STATE keyboard_state;
    al_get_keyboard_state(&keyboard_state);

    int dx    = al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)
              - al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT);
    int dy    = al_key_down(&keyboard_state, ALLEGRO_KEY_UP);
    int shoot = al_key_down(&keyboard_state, ALLEGRO_KEY_X);

    // movement
    m_ang += dx * 0.075;
    if (dy) {
        m_vel.x -= std::sin(m_ang) * 0.05f;
        m_vel.y += std::cos(m_ang) * 0.05f;
        ++m_thrust;
    }
    else {
        m_thrust = 0;
    }

    m_vel *= 0.99f;
    m_pos += m_vel;
    warp_pos();

    float speed = glm::length(m_vel);
    const float max_speed = 5;
    if (speed > max_speed) {
        m_vel *= max_speed / speed;
    }

    // shooting
    if (shoot && m_shoot_delay == 0) {
        m_shoot_delay = 10;
        world.spawn(std::make_unique<Bullet>(m_pos, m_ang));
    }
    if (m_shoot_delay > 0) --m_shoot_delay;

}


void Ship::collision(Entity& other) {
//    if (Asteroid* a = dynamic_cast<Asteroid*>(&other)) {
//    }
}


void Ship::draw(const ALLEGRO_TRANSFORM& transform) {
    if (m_thrust % 4 > 1) {
        ALLEGRO_TRANSFORM t;
        al_identity_transform(&t);
        al_rotate_transform(&t, m_ang);
        al_translate_transform(&t, m_pos.x, m_pos.y);
        al_compose_transform(&t, &transform);
        al_use_transform(&t);
        al_draw_filled_pieslice(0, -3, 3, ALLEGRO_PI + 0.2, ALLEGRO_PI - 0.4, al_map_rgb(255, 255, 255));
    }

    al_use_transform(&transform);
    al_draw_polygon((float*) &m_transformed_vertices[0], 4,
                    ALLEGRO_LINE_JOIN_ROUND, al_map_rgb(255, 255, 255), 1, 0);
}

