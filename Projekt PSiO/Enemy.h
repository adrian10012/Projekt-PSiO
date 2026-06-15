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
    sf::CircleShape shape;
    bool isInvulnerable = false;

public:
    Enemy(sf::Vector2f startPos, sf::Color color, float s, float startCooldown, float maxHpValue, float radius);
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
    float attackResetTime;

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
    float attackResetTime;

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
    float attackResetTime;

public:
    ShooterEnemy(sf::Vector2f startPos, sf::Color color, float maxHp, float s, float radius, float bSpeed, float bDmg, float cd);

    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
    void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) override;
};

class FirstMeleeEnemy : public MeleeEnemy {
public:
    FirstMeleeEnemy(sf::Vector2f startPos);
};

class SecondMeleeEnemy : public MeleeEnemy {
public:
    SecondMeleeEnemy(sf::Vector2f startPos);
};

class FirstThrowerEnemy : public ThrowerEnemy {
public:
    FirstThrowerEnemy(sf::Vector2f startPos);
};

class SecondThrowerEnemy : public ThrowerEnemy {
public:
    SecondThrowerEnemy(sf::Vector2f startPos);
};

class FirstShooterEnemy : public ShooterEnemy {
public:
    FirstShooterEnemy(sf::Vector2f startPos);
};

class SecondShooterEnemy : public ShooterEnemy {
public:
    SecondShooterEnemy(sf::Vector2f startPos);
};