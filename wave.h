#pragma once
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Player.h"
#include "Path.h"
#include "Enemy.h"
#include "Basicenemy.h"
#include "fastenemy.h"
#include "Tankenemy.h"
#include "peka.h"
#include "map.h"

class wave {
private:
    int wavecount;
    enemy** e;
    int total;
    int spawnIndex;
    float lastSpawn;
    float spawnDelay;
    Map* map;
    sf::Clock clock;
    bool waveActive;        // FIX: tracks whether spawning has begun

public:
    wave(Map* m) {
        map = m;
        wavecount = 0;
        total = 5;
        spawnIndex = 0;
        spawnDelay = 1.5f;
        lastSpawn = -999.f;  // FIX: force first enemy to spawn immediately
        waveActive = false;
        e = new enemy * [total];
        for (int i = 0; i < total; i++)
            e[i] = nullptr;
    }

    void spawnEnemy() {
        float currentTime = clock.getElapsedTime().asSeconds();
        if (spawnIndex >= total) return;
        if (currentTime - lastSpawn < spawnDelay) return;

        int pathIndex = rand() % map->getCount();
        Path& p = map->getPath(pathIndex);
        enemy* newEnemy = nullptr;

        switch (wavecount) {
        case 0: newEnemy = new basicenemy(&p); break;
        case 1: newEnemy = new fastenemy(&p);  break;
        case 2: newEnemy = new tankenemy(&p);  break;
        case 3: newEnemy = new peka(&p);       break;
        case 4:
            switch (rand() % 4) {
            case 0: newEnemy = new basicenemy(&p); break;
            case 1: newEnemy = new fastenemy(&p);  break;
            case 2: newEnemy = new tankenemy(&p);  break;
            case 3: newEnemy = new peka(&p);       break;
            }
            break;
        }

        if (!newEnemy) return;

        e[spawnIndex] = newEnemy;
        e[spawnIndex]->setposition(
            p.get(0).getxaxis(),
            p.get(0).getyaxis()
        );
        spawnIndex++;
        lastSpawn = currentTime;
        waveActive = true;   // FIX: wave has started
    }

    void update() {
        spawnEnemy();

        // FIX: do not check for wave completion until spawning has begun
        if (!waveActive) return;

        // FIX: only check enemies that have actually been spawned
        bool allDead = true;
        for (int i = 0; i < spawnIndex; i++) {
            if (e[i] && e[i]->isalive()) {
                allDead = false;
                break;
            }
        }

        // FIX: all enemies must be spawned AND dead before advancing
        if (spawnIndex >= total && allDead) {
            for (int i = 0; i < total; i++) {
                delete e[i];
                e[i] = nullptr;
            }
            delete[] e;

            wavecount++;
            if (wavecount > 4) return;

            total += 3;
            e = new enemy * [total];
            for (int i = 0; i < total; i++)
                e[i] = nullptr;

            spawnIndex = 0;
            waveActive = false;                              // FIX: reset for next wave
            lastSpawn = clock.getElapsedTime().asSeconds(); // small delay before next wave
        }
    }

    void render(sf::RenderWindow& window) {
        for (int i = 0; i < total; i++)
            if (e[i] && e[i]->isalive())
                e[i]->render(window);
    }

    enemy** getEnemies() { return e; }
    int getTotal() { return total; }
    int getWave() { return wavecount; }
};