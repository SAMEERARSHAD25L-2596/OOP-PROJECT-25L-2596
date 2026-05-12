#pragma once

#include <iostream>
#include <cmath>
#include <string>

#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Path.h"

class tower;

class enemy : public entity {
protected:
    float speed;
    float damagerate;
    int range;
    float originalSpeed;
    bool isStopped;

    Path* path;
    int index;              // current waypoint index

    float lasttimefire;
    float delay;

    int reward;
    int maxhp;

public:
    enemy() {
        speed = 0;
        damagerate = 0;
        range = 0;
        originalSpeed = 0;
        isStopped = false;
        path = nullptr;
        index = 0;
        lasttimefire = 0;
        delay = 0;
        reward = 0;
        maxhp = 0;
    }

    enemy(float s, float xcor, float ycor, int h, std::string n,
        float dr, int r, Path* p1, float l, float d, int re)
        : entity(xcor, ycor, h, n) {

        speed = s;
        damagerate = dr;
        range = r;
        originalSpeed = s;
        isStopped = false;

        path = p1;
        index = 1;

        lasttimefire = l;
        delay = d;

        reward = re;
        maxhp = h;

        // spawn at first waypoint
        if (path != nullptr && path->size() > 0) {
            x = path->get(0).getxaxis();
            y = path->get(0).getyaxis();
        }
    }

    virtual void move() = 0;

    bool hasReachedEnd() {
        return (path != nullptr && index >= path->size());
    }

    virtual void attack(tower* t) = 0;

    virtual bool towerinrange(tower* t) = 0;

    int getreward() {
        return reward;
    }

    int getrange() {
        return range;
    }

    int gethp() {
        return hp;
    }

    int getmaxhp() {
        return maxhp;
    }

    void setstop() {
        
            speed = originalSpeed;
            isStopped = false;
           }
    virtual void render(sf::RenderWindow& window) override {
        // Body
        sf::CircleShape body(12.f);
        body.setOrigin(12.f, 12.f);
        body.setPosition(x, y);
        body.setFillColor(sf::Color(200, 50, 50));
        window.draw(body);

        // HP bar background (dark red, full width)
        float barWidth = 30.f;
        float barHeight = 5.f;
        sf::RectangleShape hpBg(sf::Vector2f(barWidth, barHeight));
        hpBg.setFillColor(sf::Color(60, 0, 0));
        hpBg.setOutlineThickness(1.f);
        hpBg.setOutlineColor(sf::Color(0, 0, 0));
        hpBg.setPosition(x - barWidth / 2.f, y - 22.f);
        window.draw(hpBg);

        // HP bar fill (green → yellow → red based on health)
        float ratio = (maxhp > 0) ? (float)hp / (float)maxhp : 0.f;
        if (ratio < 0.f) ratio = 0.f;
        if (ratio > 1.f) ratio = 1.f;

        sf::Uint8 r = (sf::Uint8)(255 * (1.f - ratio));   // more red as hp drops
        sf::Uint8 g = (sf::Uint8)(255 * ratio);            // more green when healthy
        sf::RectangleShape hpFill(sf::Vector2f(barWidth * ratio, barHeight));
        hpFill.setFillColor(sf::Color(r, g, 0));
        hpFill.setPosition(x - barWidth / 2.f, y - 22.f);
        window.draw(hpFill);
    }
    // Add this to Enemy.h (not virtual, just a helper)
    void drawHPBar(sf::RenderWindow& window) {
        float barWidth = 30.f;
        float barHeight = 5.f;

        sf::RectangleShape hpBg(sf::Vector2f(barWidth, barHeight));
        hpBg.setFillColor(sf::Color(60, 0, 0));
        hpBg.setOutlineThickness(1.f);
        hpBg.setOutlineColor(sf::Color(0, 0, 0));
        hpBg.setPosition(x - barWidth / 2.f, y - 22.f);
        window.draw(hpBg);

        float ratio = (maxhp > 0) ? (float)hp / (float)maxhp : 0.f;
        if (ratio < 0.f) ratio = 0.f;
        if (ratio > 1.f) ratio = 1.f;
        sf::Uint8 r = (sf::Uint8)(255 * (1.f - ratio));
        sf::Uint8 g = (sf::Uint8)(255 * ratio);
        sf::RectangleShape hpFill(sf::Vector2f(barWidth * ratio, barHeight));
        hpFill.setFillColor(sf::Color(r, g, 0));
        hpFill.setPosition(x - barWidth / 2.f, y - 22.f);
        window.draw(hpFill);
    }

    // ⚠️ SFML FIXED VERSION (no Raylib dependency)
    virtual void stop(tower* t, float currentTime) = 0;

    virtual ~enemy() {}
};