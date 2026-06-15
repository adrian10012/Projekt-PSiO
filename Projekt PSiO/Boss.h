#pragma once
#include "Enemy.h"
#include <vector>

class BossFirst : public Enemy {
private:
    int state = 0;
    float stateTimer = 2.0f;
    int chargeCount = 0;
    sf::Vector2f dashDir;

public:
    BossFirst(sf::Vector2f startPos);

    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
    void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) override;
};

class BossSecond : public MeleeEnemy {
private:
    float splashTimer = 1.0f;
    int splitLevel;

public:
    BossSecond(sf::Vector2f startPos, int level = 3);
    int getSplitLevel() const;
    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
    void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) override;
};

class BossThird : public Enemy {
private:
    float dropTimer = 1.2f;
    float throwTimer = 2.5f;
    int attackPhase = 0;
    float strafeSign = 1.f;

public:
    BossThird(sf::Vector2f startPos);

    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
    void update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) override;
};