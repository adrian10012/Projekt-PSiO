#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"
#include "Splash.h"

extern const int TILE_SIZE;
extern const int GRID_W;
extern const int GRID_H;

class Enemy {
public:
    sf::Vector2f pos;
    float speed;
    float hp = 100.f;
    float attackCooldown = 0.f;
    sf::CircleShape shape;

    Enemy(sf::Vector2f startPos, sf::Color color, float s, float startCooldown);
    virtual ~Enemy() = default;

    virtual void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes);
    virtual void draw(sf::RenderWindow& window);

    bool Sees(sf::Vector2f start, sf::Vector2f target, const std::vector<std::vector<bool>>& gridWalls);
    sf::Vector2f Dijkstra(sf::Vector2f targetPos, const std::vector<std::vector<bool>>& gridWalls);
    virtual sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) = 0;
};

class MeleeEnemy : public Enemy {
    float swordAnimTimer = 0.f;
    float attackAngle = 0.f;
    sf::RectangleShape swordShape;
public:
    MeleeEnemy(sf::Vector2f startPos);
    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
    void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) override;
    void draw(sf::RenderWindow& window) override;
};

class ThrowerEnemy : public Enemy {
    int state = 0;
public:
    ThrowerEnemy(sf::Vector2f startPos);
    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
    void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) override;
};

class ShooterEnemy : public Enemy {
    float strafeSign = 1.f;
public:
    ShooterEnemy(sf::Vector2f startPos);
    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
    void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) override;
};