#include "Enemy.h"
#include "Tower.h"

void enemy::stop(tower* t, float currentTime) {

    if (!t || !t->isalive()) {
        speed = originalSpeed;
        isStopped = false;
        return;
    }

    if (towerinrange(t)) {

        // stop movement
        speed = 0.0f;
        isStopped = true;

        // attack timing control (SFML-friendly)
        if (currentTime - lasttimefire >= delay) {
            attack(t);
            lasttimefire = currentTime;
        }
    }
    else {
        // resume movement
        speed = originalSpeed;
        isStopped = false;
    }
}