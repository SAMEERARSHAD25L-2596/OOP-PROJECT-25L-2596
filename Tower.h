#pragma once
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Path.h"

class player;
class enemy;

class tower : public entity {
protected:
    int range;
    float lasttimefire;
    int level;
    int money;
    float firedelay;
    float damagerate;
    int priceforupgrade;

    // ✅ SFML VISUAL REPRESENTATION
    sf::CircleShape shape;

public:
    tower(float xcor, float ycor, int h, std::string n,
        int r, float fr, int l, int m,
        float delay, float dr)
        : entity(xcor, ycor, h, n) {

        range = r;
        lasttimefire = fr;
        level = l;
        money = m;
        firedelay = delay;
        damagerate = dr;
        priceforupgrade = 0;

        // =========================
        // SFML VISUAL SETUP
        // =========================
        shape.setRadius(12.f);
        shape.setOrigin(12.f, 12.f);
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
    }

    // =========================
    // PURE VIRTUAL FUNCTIONS
    // =========================
    virtual void update(player& p1) = 0;
    virtual void attack(enemy* e) = 0;
    virtual bool enemeyinrange(enemy* e) = 0;
    virtual bool canfire(float current) = 0;
    virtual void resetFireTime(float currentTime) = 0;
    virtual void takedamage(float damage) = 0;

    // =========================
    // GETTERS
    // =========================
    int getmoney() { return money; }
    int getrange() { return range; }
    int getlevel() { return level; }

    float getx() { return x; }
    float gety() { return y; }

    // =========================
    // POSITION SETTERS
    // =========================
    void setx(float xcor) {
        x = xcor;
        shape.setPosition(x, y);
    }

    void sety(float ycor) {
        y = ycor;
        shape.setPosition(x, y);
    }

    // =========================
    // GRID PLACEMENT (SFML)
    // =========================
    bool inplace(sf::RenderWindow& window) {
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        int grid = 40;

        int snappedX = (mouse.x / grid) * grid;
        int snappedY = (mouse.y / grid) * grid;

        return (x == snappedX && y == snappedY);
    }

    // =========================
    // SFML RENDER FUNCTION (IMPORTANT FIX)
    // =========================
    virtual void render(sf::RenderWindow& window) {
        shape.setPosition(x, y);
        window.draw(shape);
    }
    virtual void color(Path& p, player& pl, sf::RenderWindow& window);
    // =========================
    // OPTIONAL: RANGE DEBUG (REMOVE LATER)
    // =========================
    void drawRange(sf::RenderWindow& window) {
        sf::CircleShape rangeCircle;
        rangeCircle.setRadius(range);
        rangeCircle.setOrigin(range, range);
        rangeCircle.setPosition(x, y);
        rangeCircle.setFillColor(sf::Color(255, 255, 255, 30));
        window.draw(rangeCircle);
    }
};