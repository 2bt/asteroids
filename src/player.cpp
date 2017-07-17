// vim: ts=4 sw=4 sts=4 et
#include "player.hpp"
#include "ship.hpp"
#include "world.hpp"

Player::Player() {
	world.spawn(std::make_unique<Ship>(*this, false));
}

void Player::update() {

	// respawn
	if (!m_ship && m_lives > 0) {
		world.spawn(std::make_unique<Ship>(*this, true));
	}

	// input
    static ALLEGRO_KEYBOARD_STATE keyboard_state;
    al_get_keyboard_state(&keyboard_state);
    m_input.dx    = al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)
                  - al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT);
    m_input.dy    = al_key_down(&keyboard_state, ALLEGRO_KEY_UP);
    m_input.shoot = al_key_down(&keyboard_state, ALLEGRO_KEY_X);
}

