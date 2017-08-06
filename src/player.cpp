// vim: ts=4 sw=4 sts=4 et
#include "player.hpp"
#include "ship.hpp"
#include "world.hpp"


Player::Player() {
    m_score = 0;
    m_lives = 3;
}


void Player::init() {
    m_joystick = nullptr;
    if (al_get_num_joysticks() > 0) {
        m_joystick = al_get_joystick(0);
    }

    world.spawn(std::make_unique<Ship>(*this, false));
}

void Player::update() {

    // respawn
    if (!m_ship && m_lives > 0) {
        world.spawn(std::make_unique<Ship>(*this, true));
    }

    // input
    m_input.old_shoot = m_input.shoot;
    static ALLEGRO_KEYBOARD_STATE keyboard_state;
    al_get_keyboard_state(&keyboard_state);
    m_input.dx    = al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)
                  - al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT);
    m_input.dy    = al_key_down(&keyboard_state, ALLEGRO_KEY_UP);
    m_input.shoot = al_key_down(&keyboard_state, ALLEGRO_KEY_X);


    if (m_joystick) {
        ALLEGRO_JOYSTICK_STATE j;
        al_get_joystick_state(m_joystick, &j);

        m_input.dx = j.stick[0].axis[0];
        m_input.dy = -j.stick[0].axis[1];
        m_input.shoot = j.button[0];
    }
}

