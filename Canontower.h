#pragma once
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Tower.h"
#include "Enemy.h"
#include "Player.h"

class canontower : public tower {
private:
    sf::RectangleShape shape;
public:
    canontower(float xcor, float ycor)
        : tower(xcor, ycor, 200, "Canons", 100, 0.f, 1, 100, 0.5f, 40.0f) {
        // FIX: lasttimefire=0 is fine — gameClock starts at 0 and
        //      canfire() will pass after 0.5s of real time naturally
        priceforupgrade = 100;
        shape.setSize(sf::Vector2f(34.f, 34.f));
        shape.setFillColor(sf::Color(90, 90, 90));
        shape.setOrigin(17.f, 17.f);
    }

    void takedamage(float damage) override {
        hp -= (int)damage;
        if (hp < 0) hp = 0;
    }

    void attack(enemy* e) override {
        e->takedamage(damagerate);  // deals 40 damage per shot
    }

    bool enemeyinrange(enemy* e) override {
        float dx = e->getx() - x;
        float dy = e->gety() - y;
        return std::sqrt(dx * dx + dy * dy) <= (float)range;
    }

    bool canfire(float currentTime) override {
        return (currentTime - lasttimefire) >= firedelay;
    }

    void resetFireTime(float currentTime) override {
        lasttimefire = currentTime;
    }

    void render(sf::RenderWindow& window) override {
        shape.setPosition(x, y);
        window.draw(shape);
    }

    void update(player& p1) override {
        if (level > 4) return;
        if (p1.getgold() >= priceforupgrade) {
            hp += 50;
            damagerate += 10;
            range += 5;
            level++;
            p1.spendgold(priceforupgrade);
            priceforupgrade += 30;
        }
    }

    int getlevel() { return level; }
};