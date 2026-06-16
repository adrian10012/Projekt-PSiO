#pragma once
#include <SFML/Graphics.hpp>

class Splash {
private:
    sf::Vector2f pos;
    float currRadius;
    float maxRadius;
    float lifetime;
    float maxLifetime;
    bool isBomb;
    sf::CircleShape shape;

    sf::Sprite sprite;
    int currentFrame = 0;
    float frameTimer = 0.f;
    float frameDuration;

    static sf::Texture& getExplosionTexture() {
        static sf::Texture tex;
        static bool loaded = false;
        if (!loaded) {
            tex.loadFromFile("textures/particles/Explosion_02.png");
            loaded = true;
        }
        return tex;
    }

    static sf::Texture& getPoisonTexture() {
        static sf::Texture tex;
        static bool loaded = false;
        if (!loaded) {
            tex.loadFromFile("textures/particles/1112.png");
            loaded = true;
        }
        return tex;
    }

public:
    Splash(sf::Vector2f p, bool bomb) : pos(p), isBomb(bomb) {
        currRadius = 5.f;

        shape.setFillColor(sf::Color::Transparent);
        shape.setPosition(pos);

        if (isBomb) {
            maxRadius = 75.f;
            maxLifetime = 0.25f;
            lifetime = maxLifetime;
            frameDuration = maxLifetime / 10.f;

            sf::Texture& tex = getExplosionTexture();
            sprite.setTexture(tex);
            int w = tex.getSize().x / 10;
            int h = tex.getSize().y;
            sprite.setTextureRect(sf::IntRect(0, 0, w, h));
            sprite.setOrigin(w / 2.f, h / 2.f);
            sprite.setPosition(pos);
        }
        else {
            maxRadius = 50.f;
            maxLifetime = 8.0f;
            lifetime = maxLifetime;

            sf::Texture& tex = getPoisonTexture();
            sprite.setTexture(tex);

            int w = tex.getSize().x / 7;
            int h = tex.getSize().y / 9;

            sprite.setTextureRect(sf::IntRect(0, 3 * h, w, h));
            sprite.setOrigin(w / 2.f, h / 2.f);
            sprite.setPosition(pos);
        }
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

        if (isBomb) {
            frameTimer += dt;
            if (frameTimer >= frameDuration) {
                frameTimer = 0.f;
                if (currentFrame < 9) currentFrame++;
            }

            int w = sprite.getTexture()->getSize().x / 10;
            int h = sprite.getTexture()->getSize().y;
            sprite.setTextureRect(sf::IntRect(currentFrame * w, 0, w, h));

            float scale = (currRadius * 2.5f) / w;
            sprite.setScale(scale, scale);
        }
        else {
            float progress = 1.0f - (lifetime / maxLifetime);
            int totalFrames = 7;
            currentFrame = static_cast<int>(progress * totalFrames);

            if (currentFrame > 6) currentFrame = 6;
            if (currentFrame < 0) currentFrame = 0;

            int w = sprite.getTexture()->getSize().x / 7;
            int h = sprite.getTexture()->getSize().y / 9;

            sprite.setTextureRect(sf::IntRect(currentFrame * w, 3 * h, w, h));

            float scale = (currRadius * 2.5f) / w;
            sprite.setScale(scale, scale);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};