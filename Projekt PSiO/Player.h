#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    sf::Vector2f pos;
    sf::CircleShape shape;
    float speed;

    Player();
    void update(float dt);
    void draw(sf::RenderWindow& window);
};