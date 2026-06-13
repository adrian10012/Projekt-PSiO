#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class Bullet {
private:
    sf::Vector2f pos;
    sf::Vector2f vel;
    float lifetime;
    float maxLifetime;
    float damage;
    bool isEnemy;
    bool isSplash;
    bool isBomb;
    sf::RectangleShape shape;

public:
    Bullet(sf::Vector2f p, sf::Vector2f dir, float dmg, bool enemy, bool splash, bool bomb, float speed, float radius)
        : pos(p), damage(dmg), isEnemy(enemy), isSplash(splash), isBomb(bomb) {

        vel = dir * speed;
        maxLifetime = splash ? 0.6f : 1.5f;
        lifetime = maxLifetime;

        if (splash) {
            shape.setSize(sf::Vector2f(16.f, 16.f));
            shape.setOrigin(8.f, 8.f);
            shape.setFillColor(isBomb ? sf::Color(255, 60, 0) : sf::Color(0, 220, 0));
        }
        else {
            shape.setSize(sf::Vector2f(radius * 2.f, radius * 2.f));
            shape.setOrigin(radius, radius);
            shape.setFillColor(isEnemy ? sf::Color::Yellow : sf::Color::Cyan);

            float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
            shape.setRotation(angle);
        }
    }

    sf::Vector2f getPos() const { return pos; }
    float getLifetime() const { return lifetime; }
    float getDamage() const { return damage; }
    bool getIsEnemy() const { return isEnemy; }
    bool getIsSplash() const { return isSplash; }
    bool getIsBomb() const { return isBomb; }
    sf::RectangleShape getShape() const { return shape; }

    void setLifetime(float newLifetime) { lifetime = newLifetime; }

    void update(float dt, const std::vector<sf::RectangleShape>& walls) {
        if (lifetime <= 0.f) return;

        pos += vel * dt;
        lifetime -= dt;

        if (isSplash) {
            float progress = 1.0f - (lifetime / maxLifetime);
            float arcHeight = std::sin(progress * 3.14159f) * 120.f;
            shape.setPosition(pos.x, pos.y - arcHeight);
            shape.rotate(360.f * dt);
        }
        else {
            shape.setPosition(pos);
            for (const auto& wall : walls) {
                if (wall.getGlobalBounds().intersects(shape.getGlobalBounds())) {
                    lifetime = 0.f;
                    break;
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};