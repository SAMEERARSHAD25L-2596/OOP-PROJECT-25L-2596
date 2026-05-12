#pragma once
#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "Tower.h"
#include "Enemy.h"
#include "Player.h"

class machineguntower : public tower {
private:
    sf::RectangleShape shape;

public:
    machineguntower(float xcor, float ycor)
        : tower(xcor, ycor, 150, "Machine Gun", 80, 0, 1, 75, 0.2f, 10.0f) {

        priceforupgrade = 75;

        // SFML visual setup
        shape.setSize(sf::Vector2f(28.f, 28.f));
        shape.setFillColor(sf::Color(70, 70, 70)); // dark gray machine gun
        shape.setOrigin(14.f, 14.f);
    }

    void takedamage(float damage) override {
        hp -= (int)damage;
        if (hp < 0) {
            hp = 0;
        }
    }

    void attack(enemy* e) override {
        e->takedamage(damagerate);
    }

    bool enemeyinrange(enemy* e) override {
        float dx = e->getx() - x;
        float dy = e->gety() - y;

        float distance = std::sqrt(dx * dx + dy * dy);

        return distance <= range;
    }

    // ✅ FULL SFML RENDER
    void render(sf::RenderWindow& window) override {
        shape.setPosition(x, y);
        window.draw(shape);
    }

    bool canfire(float currentTime) override {
        return (currentTime - lasttimefire) >= firedelay;
    }

    void resetFireTime(float currentTime) override {
        lasttimefire = currentTime;
    }

    int getlevel() {
        return level;
    }

    void update(player& p1) override {
        if (level > 4) return;

        if (p1.getgold() >= priceforupgrade) {
            hp += 60;
            damagerate += 10;
            range += 5;
            level++;

            p1.spendgold(priceforupgrade);
            priceforupgrade += 30;
        }
    }
};