#pragma once
#include <SFML/Graphics.hpp>

class Splash {
public:
    sf::Vector2f pos;
    float currentRadius;
    float maxRadius;
    float lifetime;
    bool isFire;
    sf::CircleShape shape;

    Splash(sf::Vector2f p, bool fire) : pos(p), isFire(fire) {
        currentRadius = fire ? 10.f : 5.f;
        maxRadius = fire ? 60.f : 45.f;
        lifetime = fire ? 5.0f : 4.0f;

        shape.setFillColor(fire ? sf::Color(255, 100, 0, 150) : sf::Color(0, 255, 0, 120));
        shape.setPosition(pos);
    }

    void update(float dt) {
        float growSpeed = isFire ? 350.f : 80.f;
        if (currentRadius < maxRadius) {
            currentRadius += growSpeed * dt;
            if (currentRadius > maxRadius) currentRadius = maxRadius;
        }
        shape.setRadius(currentRadius);
        shape.setOrigin(currentRadius, currentRadius);
        lifetime -= dt;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};