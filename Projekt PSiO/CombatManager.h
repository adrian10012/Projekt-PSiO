#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "Enemy.h"
#include "Bullet.h"
#include "Splash.h"
#include "Boss.h"

inline void cleanupCombat(std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Bullet>& bullets, std::vector<Splash>& splashes, std::vector<std::unique_ptr<Enemy>>& newSpawns) {
    for (auto& b : bullets) {
        if (b.lifetime <= 0.f && b.isSplashProjectile) {
            splashes.emplace_back(b.pos, b.isFireSplash);
        }
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return b.lifetime <= 0.f; }), bullets.end());
    splashes.erase(std::remove_if(splashes.begin(), splashes.end(), [](const Splash& s) { return s.lifetime <= 0.f; }), splashes.end());
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [&newSpawns](const std::unique_ptr<Enemy>& e) {
        if (e->hp <= 0.f) {
            BossSecond* szlam = dynamic_cast<BossSecond*>(e.get());
            if (szlam && szlam->splitLevel > 1) {
                newSpawns.push_back(std::make_unique<BossSecond>(szlam->pos + sf::Vector2f(-20.f, 0.f), szlam->splitLevel - 1));
                newSpawns.push_back(std::make_unique<BossSecond>(szlam->pos + sf::Vector2f(20.f, 0.f), szlam->splitLevel - 1));
            }
            return true;
        }
        return false;
        }), enemies.end());
}