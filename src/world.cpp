// vim: ts=4 sw=4 sts=4 et
#include "world.hpp"
#include "ship.hpp"
#include "asteroid.hpp"
#include "particle.hpp"
#include <ctime>


void World::init() {

    srand(time(nullptr));

    m_entities.clear();

    spawn(std::make_unique<Ship>());
    for (int i = 0; i < 5; ++i) {
        glm::vec2 pos = {
            random_float(0, WIDTH),
            random_float(0, HEIGHT)
        };
        spawn( std::make_unique<Asteroid>(pos, 3));
    }
}


Entity& World::spawn(Entity::Ptr&& e) {
    e->update_transformed_vertices();
    m_new_entities.emplace_back(std::move(e));
    return *m_new_entities.back();
}


void World::spawn_explosion(const glm::vec2& pos, float r) {
    for (int i = 0; i < r * r / 20; ++i) {
        float ang = random_float(0, 2 * ALLEGRO_PI);
        float speed = random_float(0, 1.5);
        glm::vec2 vel = glm::vec2(std::sin(ang), std::cos(ang)) * speed;
        spawn(std::make_unique<Particle>(pos + vel * r * 0.5f, vel, random_float(20, 40)));
    }
}


void World::update() {
//    // XXX
//    ALLEGRO_MOUSE_STATE mouse;
//    al_get_mouse_state(&mouse);
//    glm::vec2 p = { mouse.x, mouse.y };
//    al_transform_coordinates(al_get_current_inverse_transform(), &p.x, &p.y);

    // update
    for (auto it = m_entities.begin(); it != m_entities.end();) {
        (*it)->update();
        if (!(*it)->is_alive()) {
            it = m_entities.erase(it);
            continue;
        }
        (*it)->update_transformed_vertices();
        ++it;
    }

    // collision
    for (int i = 0; i < (int) m_entities.size() - 1; ++i) {
        Entity& e1 = *m_entities[i];
        for (int j = i + 1; j < (int) m_entities.size(); ++j) {
            Entity& e2 = *m_entities[j];
            if (!e1.is_alive()) break;
            if (!e2.is_alive()) continue;
            if (e1.check_collision(e2)) {
                e1.collision(e2);
                e2.collision(e1);
            }
        }
    }

    // append new entities
    m_entities.resize(m_entities.size() + m_new_entities.size());
    std::move(m_new_entities.begin(), m_new_entities.end(), m_entities.end() - m_new_entities.size());
    m_new_entities.clear();
}


void World::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    ALLEGRO_TRANSFORM old_transform;
    al_copy_transform(&old_transform, al_get_current_transform());


    for (int sx = -1; sx <= 0; ++sx)
    for (int sy = -1; sy <= 0; ++sy) {

        ALLEGRO_TRANSFORM t;
        al_identity_transform(&t);
        al_translate_transform(&t, sx * WIDTH, sy * HEIGHT);
        al_compose_transform(&t, &old_transform);

        for (Entity::Ptr& e : m_entities) {
            e->draw(t);

            // bounding circle
            //al_use_transform(&t);
            //al_draw_circle(e->pos().x, e->pos().y, e->radius(), al_map_rgb(100, 100, 0), 0.5);
        }
    }
    al_use_transform(&old_transform);


//    //XXX
//    update();


    al_flip_display();
}


World world;
