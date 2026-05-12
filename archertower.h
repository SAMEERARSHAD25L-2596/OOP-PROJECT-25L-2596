#pragma once
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Tower.h"
#include "Enemy.h"
#include "Player.h"

using namespace std;

class archertower : public tower {
public:
    archertower(float xcor, float ycor)
        : tower(xcor, ycor, 300, "ArcherTower", 90, 0, 1, 80, 0.3f, 30.0f) {
        priceforupgrade = 80;
    }

    void takedamage(float damage) override {
        hp -= int(damage);
        if (hp < 0) hp = 0;
    }

    void attack(enemy* e) override {
        e->takedamage(damagerate);
    }

    bool enemeyinrange(enemy* e) override {
        float dx = e->getx() - x;
        float dy = e->gety() - y;
        float distance = sqrt(dx * dx + dy * dy);
        return distance <= range;
    }

    // ✅ FIXED SFML RENDER
    void render(sf::RenderWindow& window) override {
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Yellow);
        window.draw(shape);
    }

    bool canfire(float currenttime) override {
        return (currenttime - lasttimefire) >= firedelay;
    }

    void resetFireTime(float currentTime) override {
        lasttimefire = currentTime;
    }

    void update(player& p1) override {
        if (level > 4) return;

        if (p1.getgold() >= priceforupgrade) {
            hp += 70;
            damagerate += 8;
            range += 10;
            level++;
            p1.spendgold(priceforupgrade);
            priceforupgrade += 25;
        }
    }
};