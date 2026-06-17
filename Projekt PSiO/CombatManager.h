#pragma once
#include <vector>
#include <memory>
#include "Enemy.h"
#include "Boss.h"
#include "Bullet.h"
#include "Splash.h"

inline void cleanupCombat(std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Bullet>& bullets, std::vector<Splash>& splashes, std::vector<std::unique_ptr<Enemy>>& newSpawns, int& playerGold) {
    auto bulletIterator = bullets.begin();
    while (bulletIterator != bullets.end()) {
        if (bulletIterator->getLifetime() <= 0.f) {
            if (bulletIterator->getIsSplash()) splashes.emplace_back(bulletIterator->getPos(), bulletIterator->getIsBomb());
            bulletIterator = bullets.erase(bulletIterator);
        }
        else bulletIterator++;
    }

    auto splashIterator = splashes.begin();
    while (splashIterator != splashes.end()) {
        if (splashIterator->getLifetime() <= 0.f) splashIterator = splashes.erase(splashIterator);
        else splashIterator++;
    }

    auto enemyIterator = enemies.begin();
    while (enemyIterator != enemies.end()) {
        if ((*enemyIterator)->getHp() <= 0.f) {

            playerGold += static_cast<int>((*enemyIterator)->getShape().getRadius()) * 3;

            BossSecond* bossSecond = dynamic_cast<BossSecond*>(enemyIterator->get());
            if (bossSecond) {
                int lvl = bossSecond->getSplitLevel();
                if (lvl > 1) {
                    sf::Vector2f p = bossSecond->getPos();
                    newSpawns.push_back(std::make_unique<BossSecond>(sf::Vector2f(p.x - 30.f, p.y), lvl - 1));
                    newSpawns.push_back(std::make_unique<BossSecond>(sf::Vector2f(p.x + 30.f, p.y), lvl - 1));
                }
            }
            enemyIterator = enemies.erase(enemyIterator);
        }
        else enemyIterator++;
    }
}