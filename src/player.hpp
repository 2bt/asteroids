// vim: ts=4 sw=4 sts=4 et
#pragma once
#include <allegro5/allegro.h>


class Ship;


struct Input {
    float dx;
    float dy;
    bool  shoot;
    bool  old_shoot;

    bool button_down() const { return shoot && !old_shoot; }
};


class Player {
public:
    void init();
    void update();
    const Input& input() const { return m_input; }
    void set_ship(Ship* ship) {
        m_ship = ship;
        if (!m_ship) --m_lives;
    }
    int score() const { return m_score; }
    int lives() const { return m_lives; }
    void inc_score(int s) { m_score += s; }


private:
    Ship*             m_ship  = nullptr;
    int               m_score = 0;
    int               m_lives = 3;
    Input             m_input;
    ALLEGRO_JOYSTICK* m_joystick;
};
