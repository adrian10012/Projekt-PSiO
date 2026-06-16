#pragma once
#include "Enemy.h"
#include <vector>

class BossFirst : public Enemy {
private:
    int state = 0;
    float stateTimer = 2.0f;
    int chargeCount = 0;
    sf::Vector2f dashDir;
    sf::Sprite sprite;
    sf::Clock animClock;
    int currentFrame = 0;
    float frameTimer = 0.f;
    float frameDuration = 0.15f;

    static sf::Texture& getTexture() {
        static sf::Texture tex;
        static bool loaded = false;
        if (!loaded) {
            tex.loadFromFile("textures/moby/HumongousEttin.png");
            loaded = true;
        }
        return tex;
    }

public:
    BossFirst(sf::Vector2f startPos);

    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
    void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) override;

    void draw(sf::RenderWindow& window) override {
        sf::Texture& tex = getTexture();
        sprite.setTexture(tex);

        int w = tex.getSize().x / 4;
        int h = tex.getSize().y;

        float dt = animClock.restart().asSeconds();
        frameTimer += dt;
        if (frameTimer >= frameDuration) {
            frameTimer = 0.f;
            currentFrame = (currentFrame + 1) % 4;
        }

        sprite.setTextureRect(sf::IntRect(currentFrame * w, 0, w, h));
        float scale = (shape.getRadius() * 2.5f) / w;
        sprite.setScale(scale, scale);
        sprite.setOrigin(w / 2.f, h / 2.f);
        sprite.setPosition(pos);
        window.draw(sprite);

        sf::Color oldColor = shape.getFillColor();
        shape.setFillColor(sf::Color::Transparent);
        Enemy::draw(window);
        shape.setFillColor(oldColor);
    }
};

class BossSecond : public MeleeEnemy {
private:
    float splashTimer = 1.0f;
    int splitLevel;
    sf::Sprite sprite;
    sf::Clock animClock;
    int currentFrame = 0;
    float frameTimer = 0.f;
    float frameDuration = 0.15f;

    static sf::Texture& getTexture() {
        static sf::Texture tex;
        static bool loaded = false;
        if (!loaded) {
            tex.loadFromFile("textures/moby/DeathSlime.png");
            loaded = true;
        }
        return tex;
    }

public:
    BossSecond(sf::Vector2f startPos, int level = 3);
    int getSplitLevel() const;
    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
    void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) override;

    void draw(sf::RenderWindow& window) override {
        sf::Texture& tex = getTexture();
        sprite.setTexture(tex);

        int w = tex.getSize().x / 4;
        int h = tex.getSize().y;

        float dt = animClock.restart().asSeconds();
        frameTimer += dt;
        if (frameTimer >= frameDuration) {
            frameTimer = 0.f;
            currentFrame = (currentFrame + 1) % 4;
        }

        sprite.setTextureRect(sf::IntRect(currentFrame * w, 0, w, h));
        float scale = (shape.getRadius() * 2.5f) / w;
        sprite.setScale(scale, scale);
        sprite.setOrigin(w / 2.f, h / 2.f);
        sprite.setPosition(pos);
        window.draw(sprite);

        sf::Color oldColor = shape.getFillColor();
        shape.setFillColor(sf::Color::Transparent);
        MeleeEnemy::draw(window);
        shape.setFillColor(oldColor);
    }
};

class BossThird : public ShooterEnemy {
private:
    float dropTimer = 1.2f;
    float throwTimer = 2.5f;
    int attackPhase = 0;
    sf::Sprite sprite;
    sf::Clock animClock;
    int currentFrame = 0;
    float frameTimer = 0.f;
    float frameDuration = 0.15f;

    static sf::Texture& getTexture() {
        static sf::Texture tex;
        static bool loaded = false;
        if (!loaded) {
            tex.loadFromFile("textures/moby/CrimsonSlaad.png");
            loaded = true;
        }
        return tex;
    }

public:
    BossThird(sf::Vector2f startPos);

    void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) override;

    void draw(sf::RenderWindow& window) override {
        sf::Texture& tex = getTexture();
        sprite.setTexture(tex);

        int w = tex.getSize().x / 4;
        int h = tex.getSize().y;

        float dt = animClock.restart().asSeconds();
        frameTimer += dt;
        if (frameTimer >= frameDuration) {
            frameTimer = 0.f;
            currentFrame = (currentFrame + 1) % 4;
        }

        sprite.setTextureRect(sf::IntRect(currentFrame * w, 0, w, h));
        float scale = (shape.getRadius() * 2.5f) / w;
        sprite.setScale(scale, scale);
        sprite.setOrigin(w / 2.f, h / 2.f);
        sprite.setPosition(pos);
        window.draw(sprite);

        sf::Color oldColor = shape.getFillColor();
        shape.setFillColor(sf::Color::Transparent);
        ShooterEnemy::draw(window);
        shape.setFillColor(oldColor);
    }
};