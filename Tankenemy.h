#pragma once
#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "Tower.h"
#include "Enemy.h"

class tankenemy : public enemy {
private:
    sf::CircleShape shape;

public:

    tankenemy(Path* p2)
        : enemy(1.0f, 0, 0, 400, "CANON",
            4.0f, 30, p2, 0.0f, 1.5f, 150) {

        reward = 6;

        // Tank enemy appearance
        shape.setRadius(18.f);
        shape.setFillColor(sf::Color(139, 69, 19)); // Brown
        shape.setOrigin(18.f, 18.f);

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