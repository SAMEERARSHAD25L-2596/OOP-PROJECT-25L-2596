#pragma once
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Tower.h"
#include "Enemy.h"
#include "Map.h"

class player {
private:
    int gold;
    int lives;
    tower** t;
    int towercount;
    int maxtowers;
    Map* map;

public:
    player(Map* m) {
        gold = 1000;
        lives = 10;
        map = m;
        towercount = 0;
        maxtowers = 20;
        t = new tower * [maxtowers];
        for (int i = 0; i < maxtowers; i++)
            t[i] = nullptr;
        // FIX: removed erroneous "m = nullptr" line
    }

    bool isonpath(float x, float y) {
        if (map == nullptr) return false;
        for (int i = 0; i < map->getCount(); i++) {
            Path& p = map->getPath(i);
            for (int j = 0; j < p.size() - 1; j++) {
                coor a = p.get(j);
                coor b = p.get(j + 1);
                float ax = a.getxaxis(), ay = a.getyaxis();
                float bx = b.getxaxis(), by = b.getyaxis();
                // FIX: check distance to segment, not just waypoints
                float dx = bx - ax, dy = by - ay;
                float len2 = dx * dx + dy * dy;
                float t2 = 0.f;
                if (len2 > 0)
                    t2 = ((x - ax) * dx + (y - ay) * dy) / len2;
                t2 = (t2 < 0.f) ? 0.f : (t2 > 1.f ? 1.f : t2);
                float cx = ax + t2 * dx, cy = ay + t2 * dy;
                float ex = x - cx, ey = y - cy;
                if (std::sqrt(ex * ex + ey * ey) < 30.f)
                    return true;
            }
        }
        return false;
    }

    bool overlapping(float x, float y) {
        for (int i = 0; i < towercount; i++) {
            if (t[i]) {
                float dx = t[i]->getx() - x;
                float dy = t[i]->gety() - y;
                if (std::sqrt(dx * dx + dy * dy) < 40)
                    return true;
            }
        }
        return false;
    }

    bool isoutofbounds(float x, float y) {
        return (x < 0 || x > 800 || y < 40 || y > 600);
    }

    void addgold(int amount) { gold += amount; }

    bool spendgold(int amount) {
        if (gold >= amount) { gold -= amount; return true; }
        return false;
    }

    void resizearray() {
        tower** newArr = new tower * [maxtowers * 2];
        for (int i = 0; i < maxtowers; i++)
            newArr[i] = t[i];
        for (int i = maxtowers; i < maxtowers * 2; i++)
            newArr[i] = nullptr;
        delete[] t;
        t = newArr;
        maxtowers *= 2;
    }

    bool placetower(float x, float y, tower* t1) {
        if (isonpath(x, y) || isoutofbounds(x, y) || overlapping(x, y))
            return false;
        if (!spendgold(t1->getmoney()))
            return false;
        if (towercount >= maxtowers)
            resizearray();
        t[towercount] = t1;
        t[towercount]->setx(x);
        t[towercount]->sety(y);
        towercount++;
        return true;
    }

    // FIX: currenttime is now real elapsed seconds from gameClock, not dt
    void update(enemy** e, int total, float currenttime) {
        for (int i = 0; i < towercount; i++) {
            if (!t[i] || !t[i]->isalive()) continue;
            for (int j = 0; j < total; j++) {
                if (!e[j] || !e[j]->isalive()) continue;
                if (t[i]->enemeyinrange(e[j]) && t[i]->canfire(currenttime)) {
                    t[i]->attack(e[j]);
                    t[i]->resetFireTime(currenttime);
                    if (!e[j]->isalive())
                        addgold(e[j]->getreward());
                    break;
                }
            }
        }
    }

    void loselife() { lives--; }
    bool gameover() { return lives <= 0; }
    int getgold() { return gold; }
    int getlives() { return lives; }
    tower** gettower() { return t; }
    int gettowercount() { return towercount; }

    void render(sf::RenderWindow& window) {
        for (int i = 0; i < towercount; i++)
            if (t[i] && t[i]->isalive())
                t[i]->render(window);
    }

    ~player() {
        for (int i = 0; i < maxtowers; i++)
            delete t[i];
        delete[] t;
    }
};