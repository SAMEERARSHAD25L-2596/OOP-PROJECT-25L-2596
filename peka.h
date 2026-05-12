#pragma once
#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "Enemy.h"
#include "Tower.h"

class peka : public enemy {
private:
    sf::CircleShape shape;

public:

    peka(Path* p2)
        : enemy(1.5f, 0, 0, 250, "PEKA",
            15.0f, 40, p2, 0.0f, 0.8f, 200) {

        reward = 11;

        // PEKA appearance
        shape.setRadius(20.f);
        shape.setFillColor(sf::Color(150, 0, 150)); // Purple
        shape.setOrigin(20.f, 20.f);

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

        // Move toward target
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