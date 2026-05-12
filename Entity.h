#pragma once

#include <iostream>
#include <cmath>
#include <string>

#include <SFML/Graphics.hpp>

class entity {
protected:
    float x, y;
    std::string name;
    int hp;

public:
    entity() {
        x = 0;
        y = 0;
        hp = 0;
        name = "N/R";
    }

    entity(float xcor, float ycor, int h, std::string n)
        : x(xcor), y(ycor), hp(h), name(n) {
    }

    void setposition(float newx, float newy) {
        x = newx;
        y = newy;
    }

    // ✅ IMPORTANT SFML CHANGE:
    // render now requires window reference
    virtual void render(sf::RenderWindow& window) = 0;

    virtual void takedamage(float damage) = 0;

    std::string getname() {
        return name;
    }

    int gethp() {
        return hp;
    }

    float getx() {
        return x;
    }

    float gety() {
        return y;
    }

    bool isalive() {
        return hp > 0;
    }

    virtual ~entity() {}
};