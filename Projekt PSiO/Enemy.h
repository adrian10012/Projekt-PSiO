#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"
#include "Splash.h"

const int TILE_SIZE = 40;
const int GRID_W = 20;
const int GRID_H = 15;

class Enemy {
protected:
    sf::Vector2f pos;
    float speed;
    float hp;
    float maxHp;
    float attackCooldown = 0.f;
    float attackResetTime;
    sf::CircleShape shape;
    bool isInvulnerable = false;

public:
    Enemy(sf::Vector2f startPos, sf::Color color, float s, float cooldownTime, float maxHpValue, float radius);
    virtual ~Enemy() = default;

    sf::Vector2f getPos() const;
    float getHp() const;
    sf::CircleShape getShape() const;
    bool getIsInvulnerable() const;

    void setPos(sf::Vector2f newPos);
    void setHp(float newHp);

    virtual void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes);
    virtual void draw(sf::RenderWindow& window);
    bool Sees(sf::Vector2f start, sf::Vector2f target, const std::vector<std::vector<bool>>& gridWalls);
    sf::Vector2f Dijkstra(sf::Vector2f targetPos, const std::vector<std::vector<bool>>& gridWalls);
    virtual sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) = 0;
};

class MeleeEnemy : public Enemy {
protected:
    float swordAnimTimer = 0.f;
    float attackAngle = 0.f;
    sf::RectangleShape swordShape;
    float damage;

public:
    MeleeEnemy(sf::Vector2f startPos, sf::Color color, float maxHp, float s, float radius, float dmg, float cd);
    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
    void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) override;
    void draw(sf::RenderWindow& window) override;
};

class ThrowerEnemy : public Enemy {
protected:
    int state = 0;
    bool throwsBomb;

public:
    ThrowerEnemy(sf::Vector2f startPos, sf::Color color, float maxHp, float s, float radius, bool bomb, float cd);

    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
    void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) override;
};

class ShooterEnemy : public Enemy {
protected:
    float strafeSign = 1.f;
    float bulletSpeed;
    float bulletDamage;

public:
    ShooterEnemy(sf::Vector2f startPos, sf::Color color, float maxHp, float s, float radius, float bSpeed, float bDmg, float cd);

    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
    void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) override;
};


class FirstMeleeEnemy : public MeleeEnemy {
private:
    sf::Sprite sprite;
    sf::Clock animClock;
    int currentFrame = 0;
    float frameTimer = 0.f;
    float frameDuration = 0.15f;

    static sf::Texture& getTexture() {
        static sf::Texture tex;
        static bool loaded = false;
        if (!loaded) { tex.loadFromFile("textures/moby/OchreJelly.png"); loaded = true; }
        return tex;
    }
public:
    FirstMeleeEnemy(sf::Vector2f startPos);
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

class SecondMeleeEnemy : public MeleeEnemy {
private:
    sf::Sprite sprite;
    sf::Clock animClock;
    int currentFrame = 0;
    float frameTimer = 0.f;
    float frameDuration = 0.15f;

    static sf::Texture& getTexture() {
        static sf::Texture tex;
        static bool loaded = false;
        if (!loaded) { tex.loadFromFile("textures/moby/RedCap.png"); loaded = true; }
        return tex;
    }
public:
    SecondMeleeEnemy(sf::Vector2f startPos);
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

class FirstThrowerEnemy : public ThrowerEnemy {
private:
    sf::Sprite sprite;
    sf::Clock animClock;
    int currentFrame = 0;
    float frameTimer = 0.f;
    float frameDuration = 0.15f;

    static sf::Texture& getTexture() {
        static sf::Texture tex;
        static bool loaded = false;
        if (!loaded) { tex.loadFromFile("textures/moby/OchreJelly.png"); loaded = true; }
        return tex;
    }
public:
    FirstThrowerEnemy(sf::Vector2f startPos);
    void draw(sf::RenderWindow& window) override {
        sf::Texture& tex = getTexture();
        sprite.setTexture(tex);
        sprite.setColor(sf::Color(180, 255, 180)); // Zielonkawy odcieñ dla rozró¿nienia
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

class SecondThrowerEnemy : public ThrowerEnemy {
private:
    sf::Sprite sprite;
    sf::Clock animClock;
    int currentFrame = 0;
    float frameTimer = 0.f;
    float frameDuration = 0.15f;

    static sf::Texture& getTexture() {
        static sf::Texture tex;
        static bool loaded = false;
        if (!loaded) { tex.loadFromFile("textures/moby/RedCap.png"); loaded = true; }
        return tex;
    }
public:
    SecondThrowerEnemy(sf::Vector2f startPos);
    void draw(sf::RenderWindow& window) override {
        sf::Texture& tex = getTexture();
        sprite.setTexture(tex);
        sprite.setColor(sf::Color(180, 180, 255)); // Niebieskawy odcieñ dla rozró¿nienia
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

class FirstShooterEnemy : public ShooterEnemy {
private:
    sf::Sprite sprite;
    sf::Clock animClock;
    int currentFrame = 0;
    float frameTimer = 0.f;
    float frameDuration = 0.15f;

    static sf::Texture& getTexture() {
        static sf::Texture tex;
        static bool loaded = false;
        if (!loaded) { tex.loadFromFile("textures/moby/BloodshotEye.png"); loaded = true; }
        return tex;
    }
public:
    FirstShooterEnemy(sf::Vector2f startPos);
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

class SecondShooterEnemy : public ShooterEnemy {
private:
    sf::Sprite sprite;
    sf::Clock animClock;
    int currentFrame = 0;
    float frameTimer = 0.f;
    float frameDuration = 0.15f;

    static sf::Texture& getTexture() {
        static sf::Texture tex;
        static bool loaded = false;
        if (!loaded) { tex.loadFromFile("textures/moby/OcularWatcher.png"); loaded = true; }
        return tex;
    }
public:
    SecondShooterEnemy(sf::Vector2f startPos);
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