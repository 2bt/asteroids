// vim: ts=4 sw=4 sts=4 et
#pragma once


class Ship;


struct Input {
    int  dx;
    int  dy;
    bool shoot;
};


class Player {
public:

    const Input& input() const { return m_input; }
    void set_ship(Ship* ship) {
        m_ship = ship;
        if (!m_ship) --m_lives;
    }
    int score() const { return m_score; }
    void inc_score(int s) { m_score += s; }

    void update();

private:
	Ship* m_ship  = nullptr;
    int   m_score = 0;
    int   m_lives = 3;
    Input m_input;
};
