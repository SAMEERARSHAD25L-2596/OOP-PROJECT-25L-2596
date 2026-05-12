#pragma once
#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "Enemy.h"
#include "Tower.h"

class basicenemy : public enemy {
private:
    sf::CircleShape shape;

public:

    basicenemy(Path* p2)
        : enemy(2.0f, 0, 0, 100, "BARBARIAN",
            10, 50, p2, 0.0f, 1.0f, 50) {

        reward = 2;

        // Enemy appearance
        shape.setRadius(14.f);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(14.f, 14.f);

        shape.setPosition(x, y);
    }

    // ================= MOVE =================
    void move() override {

        if (path == nullptr || index >= path->size()) {
            return;
        }

        coor target = path->get(index);

        float dx = target.getxaxis() - x;
        float dy = target.getyaxis() - y;

        float dist = std::sqrt(dx * dx + dy * dy);

        // Reached waypoint
        if (dist < 5.0f) {
            index++;
            return;
        }

        // Normalize direction
        x += (dx / dist) * speed;
        y += (dy / dist) * speed;

        shape.setPosition(x, y);
    }

    // ================= ATTACK =================
    void attack(tower* t) override {

        if (t != nullptr) {
            t->takedamage(damagerate);
        }
    }

    // ================= STOP =================
    void stop(tower* t, float currentTime) override {

        // Freeze enemy
        speed = 0;
        isStopped = true;
    }

    // ================= TAKE DAMAGE =================
    void takedamage(float damage) override {

        hp -= static_cast<int>(damage);

        if (hp < 0) {
            hp = 0;
        }
    }

    // ================= RANGE CHECK =================
    bool towerinrange(tower* t) override {

        if (t == nullptr) {
            return false;
        }

        float dx = t->getx() - x;
        float dy = t->gety() - y;

        float distance = std::sqrt(dx * dx + dy * dy);

        return distance <= range;
    }

    // ================= RENDER =================
    void render(sf::RenderWindow& window) override {

        shape.setPosition(x, y);

        window.draw(shape);
        drawHPBar(window);
    }
};