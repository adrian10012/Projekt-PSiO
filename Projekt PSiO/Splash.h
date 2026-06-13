#pragma once
#include <SFML/Graphics.hpp>

class Splash {
private:
    sf::Vector2f pos;
    float currRadius;
    float maxRadius;
    float lifetime;
    bool isBomb;
    sf::CircleShape shape;

public:
    Splash(sf::Vector2f p, bool bomb) : pos(p), isBomb(bomb) {
        currRadius = 5.f;
        if (isBomb) {
            maxRadius = 75.f;
            lifetime = 0.25f;
        }
        else {
            maxRadius = 50.f;
            lifetime = 8.0f;
        }
        shape.setFillColor(isBomb ? sf::Color(255, 80, 0, 200) : sf::Color(0, 255, 0, 120));
        shape.setPosition(pos);
    }

    sf::Vector2f getPos() const { return pos; }
    float getCurrRadius() const { return currRadius; }
    float getLifetime() const { return lifetime; }
    bool getIsBomb() const { return isBomb; }

    void update(float dt) {
        float growSpeed = isBomb ? 800.f : 60.f;
        if (currRadius < maxRadius) {
            currRadius += growSpeed * dt;
            if (currRadius > maxRadius) currRadius = maxRadius;
        }
        shape.setRadius(currRadius);
        shape.setOrigin(currRadius, currRadius);
        lifetime -= dt;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};