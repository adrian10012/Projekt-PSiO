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
    sf::Sprite sprite;
    bool hasSprite; 

    static sf::Texture& getArrowTexture() {
        static sf::Texture tex;
        static bool loaded = false;
        if (!loaded) {
            tex.loadFromFile("textures/Arrow.png");
            loaded = true;
        }
        return tex;
    }

    static sf::Texture& getOrbTexture() {
        static sf::Texture tex;
        static bool loaded = false;
        if (!loaded) {
            tex.loadFromFile("textures/particles/1104.png");
            loaded = true;
        }
        return tex;
    }

public:
    Bullet(sf::Vector2f p, sf::Vector2f dir, float dmg, bool enemy, bool splash, bool bomb, float speed, float radius)
        : pos(p), damage(dmg), isEnemy(enemy), isSplash(splash), isBomb(bomb) {

        vel = dir * speed;
        maxLifetime = splash ? 0.6f : 1.5f;
        lifetime = maxLifetime;
        hasSprite = false;

        if (splash) {
            if (isBomb) {
                hasSprite = true;
                shape.setSize(sf::Vector2f(16.f, 16.f));
                shape.setOrigin(8.f, 8.f);
                shape.setFillColor(sf::Color::Transparent);

                sf::Texture& tex = getOrbTexture();
                sprite.setTexture(tex);

                int w = tex.getSize().x / 7;
                int h = tex.getSize().y / 9;

                sprite.setTextureRect(sf::IntRect(1 * w, 0, w, h));
                sprite.setOrigin(w / 2.f, h / 2.f);

                float scale = 32.f / w;
                sprite.setScale(scale, scale);
            }
            else {
                shape.setSize(sf::Vector2f(16.f, 16.f));
                shape.setOrigin(8.f, 8.f);
                shape.setFillColor(sf::Color(0, 220, 0));
            }
        }
        else {
            shape.setSize(sf::Vector2f(radius * 2.f, radius * 2.f));
            shape.setOrigin(radius, radius);
            float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
            shape.setRotation(angle);

            if (!isEnemy) {
                hasSprite = true;
                shape.setFillColor(sf::Color::Transparent);
                sf::Texture& tex = getArrowTexture();
                sprite.setTexture(tex);
                sf::FloatRect bounds = sprite.getLocalBounds();
                sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
                sprite.setRotation(angle);
            }
            else {
                shape.setFillColor(sf::Color::Yellow);
            }
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
            if (hasSprite) {
                sprite.setPosition(pos.x, pos.y - arcHeight);
                sprite.rotate(720.f * dt);
            }
            else {
                shape.rotate(360.f * dt);
            }
        }
        else {
            shape.setPosition(pos);
            if (hasSprite) {
                sprite.setPosition(pos);
            }

            for (const auto& wall : walls) {
                if (wall.getGlobalBounds().intersects(shape.getGlobalBounds())) {
                    lifetime = 0.f;
                    break;
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        if (hasSprite) {
            window.draw(sprite);
        }
        else {
            window.draw(shape);
        }
    }
};