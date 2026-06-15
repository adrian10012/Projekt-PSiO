#include "Boss.h"
#include <cmath>
#include <cstdlib>

BossFirst::BossFirst(sf::Vector2f startPos)
    : Enemy(startPos, sf::Color(100, 100, 100), 160.f, 0.f, 2500.f, 30.f) {}

sf::Vector2f BossFirst::idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) {
    if (state == 2) return pos + dashDir * 1000.f;
    if (state == 3 || state == 1) return pos;
    if (state == 0) return playerPos;
}

void BossFirst::update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) {
    stateTimer -= dt;
    if (state == 0) {
        speed = 160.f;
        shape.setFillColor(sf::Color(100, 100, 100));
        isInvulnerable = true;
        if (stateTimer <= 0.f) {
            state = 1; stateTimer = 0.5f; chargeCount = 0;
        }
    }
    else if (state == 1) {
        speed = 0.f;
        shape.setFillColor(sf::Color::Red);
        isInvulnerable = true;

        dashDir = playerPos - pos;
        float len = std::sqrt(dashDir.x * dashDir.x + dashDir.y * dashDir.y);
        if (len > 0) { dashDir.x /= len; dashDir.y /= len; }

        if (stateTimer <= 0.f) {
            state = 2; stateTimer = 0.4f;
        }
    }
    else if (state == 2) {
        speed = 1400.f;
        isInvulnerable = true;

        float distToPlayer = std::sqrt((pos.x - playerPos.x) * (pos.x - playerPos.x) + (pos.y - playerPos.y) * (pos.y - playerPos.y));
        if (distToPlayer < 45.f) playerHp -= 100.f * dt;

        if (stateTimer <= 0.f) {
            chargeCount++;
            if (chargeCount < 3) {
                state = 1; stateTimer = 0.5f;
            }
            else {
                state = 3; stateTimer = 3.5f;
            }
        }
    }
    else if (state == 3) {
        speed = 0.f;
        shape.setFillColor(sf::Color::Blue);
        isInvulnerable = false;
        if (stateTimer <= 0.f) {
            state = 0; stateTimer = 2.5f;
        }
    }

    Enemy::update(dt, playerPos, playerHp, gridWalls, bullets, splashes);
}

BossSecond::BossSecond(sf::Vector2f startPos, int level)
    : MeleeEnemy(startPos, sf::Color(50, 200, 50),
        (level == 3) ? 1500.f : (level == 2) ? 600.f : 200.f,
        (level == 3) ? 140.f : (level == 2) ? 160.f : 200.f,
        (level == 3) ? 40.f : (level == 2) ? 25.f : 12.f,
        (level == 3) ? 40.f : (level == 2) ? 25.f : 15.f,
        1.2f)
{
    splitLevel = level;
    float weaponLen = (level == 3) ? 60.f : (level == 2) ? 40.f : 25.f;
    float weaponThick = (level == 3) ? 16.f : (level == 2) ? 10.f : 6.f;
    swordShape.setSize(sf::Vector2f(weaponLen, weaponThick));
    swordShape.setOrigin(0.f, weaponThick / 2.f);
    swordShape.setFillColor(sf::Color(120, 120, 120));
}

int BossSecond::getSplitLevel() const { return splitLevel; }

sf::Vector2f BossSecond::idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) {
    return playerPos; 
}
void BossSecond::update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) {

    MeleeEnemy::update(dt, playerPos, playerHp, gridWalls, bullets, splashes);

    splashTimer -= dt;
    if (splashTimer <= 0.f) {
        bullets.emplace_back(pos, sf::Vector2f(0.f, 1.f), 0.f, true, true, false, 0.f, 1.f);
        bullets.back().setLifetime(0.01f);
        splashTimer = (splitLevel == 3) ? 0.8f : (splitLevel == 2) ? 1.5f : 2.5f;
    }
}

BossThird::BossThird(sf::Vector2f startPos)
    : Enemy(startPos, sf::Color(255, 140, 0), 170.f, 0.f, 1800.f, 25.f) {
}

sf::Vector2f BossThird::idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) {
    sf::Vector2f dir = playerPos - pos;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist > 0.f) { dir.x /= dist; dir.y /= dist; }

    sf::Vector2f strafe(-dir.y * strafeSign, dir.x * strafeSign);
    if (dist < 200.f) strafe -= dir;
    else if (dist > 350.f) strafe += dir;

    return pos + strafe * 200.f;
}

void BossThird::update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) {
    Enemy::update(dt, playerPos, playerHp, gridWalls, bullets, splashes);

    dropTimer -= dt;
    if (dropTimer <= 0.f) {
        bullets.emplace_back(pos, sf::Vector2f(0.f, 1.f), 20.f, true, true, true, 1.f, 6.f);
        dropTimer = 1.5f;
    }

    throwTimer -= dt;
    if (throwTimer <= 0.f) {
        sf::Vector2f dir = playerPos - pos;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        float baseAngle = std::atan2(dir.y, dir.x);

        attackPhase = rand() % 3;

        if (attackPhase == 0) {
            float angles[3] = { baseAngle - 0.35f, baseAngle, baseAngle + 0.35f };
            float reqSpeed = dist / 0.6f;
            for (int i = 0; i < 3; i++) {
                sf::Vector2f aim(std::cos(angles[i]), std::sin(angles[i]));
                bullets.emplace_back(pos, aim, 25.f, true, true, true, reqSpeed, 6.f);
            }
        }
        else if (attackPhase == 1) {
            for (int i = 0; i < 8; i++) {
                float a = i * (3.14159f / 4.f);
                sf::Vector2f aim(std::cos(a), std::sin(a));
                bullets.emplace_back(pos, aim, 25.f, true, true, true, 300.f, 6.f);
            }
        }
        else {
            sf::Vector2f aim(std::cos(baseAngle), std::sin(baseAngle));
            bullets.emplace_back(pos, aim, 25.f, true, true, true, dist / 0.4f, 6.f);
            bullets.emplace_back(pos, aim, 25.f, true, true, true, dist / 0.6f, 6.f);
            bullets.emplace_back(pos, aim, 25.f, true, true, true, dist / 0.8f, 6.f);
        }

        throwTimer = 2.5f;
        strafeSign *= -1.f;
    }
}