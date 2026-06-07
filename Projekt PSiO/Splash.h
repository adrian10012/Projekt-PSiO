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
        currentRadius = 5.f;
        if (isFire) {
            maxRadius = 75.f;
            lifetime = 0.25f;
        }
        else {
            maxRadius = 50.f;
            lifetime = 8.0f;
        }
        shape.setFillColor(fire ? sf::Color(255, 80, 0, 200) : sf::Color(0, 255, 0, 120));
        shape.setPosition(pos);
    }

    void update(float dt) {
        float growSpeed = isFire ? 800.f : 60.f;
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