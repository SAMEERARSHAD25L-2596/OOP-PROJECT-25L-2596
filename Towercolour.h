#include "Path.h"
#include "Entity.h"
#include "Tower.h"
#include "Enemy.h"
#include "Player.h"

#include <SFML/Graphics.hpp>

void tower::color(Path& p, player& pl, sf::RenderWindow& window) {

    sf::Vector2i mouse = sf::Mouse::getPosition(window);

    int grid = 40;

    // snap to grid

    int snappedX = (mouse.x / grid) * grid;

    int snappedY = (mouse.y / grid) * grid;

    sf::Color c = sf::Color::Green;

    // path or overlap check

    if (pl.isonpath(snappedX, snappedY) ||
        pl.overlapping(snappedX, snappedY)) {

        c = sf::Color::Red;
    }

    sf::RectangleShape preview;

    preview.setSize(sf::Vector2f(30, 30));

    preview.setPosition(
        snappedX - 15,
        snappedY - 15
    );

    preview.setFillColor(
        sf::Color(
            c.r,
            c.g,
            c.b,
            120
        )
    );

    window.draw(preview);
}