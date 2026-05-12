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
    mouse.x = (mouse.x / grid) * grid;
    mouse.y = (mouse.y / grid) * grid;

    sf::Color c = sf::Color::Green;

    // path / overlap check (same logic as yours)
    if (pl.isonpath(mouse.x, mouse.y) || pl.overlapping(mouse.x, mouse.y)) {
        c = sf::Color::Red;
    }

    // preview rectangle (SFML equivalent of DrawRectangle + Fade)
    sf::RectangleShape rect(sf::Vector2f(30.f, 30.f));
    rect.setPosition(mouse.x - 15.f, mouse.y - 15.f);

    c.a = 128; // equivalent to Fade(..., 0.5f)
    rect.setFillColor(c);

    window.draw(rect);
}