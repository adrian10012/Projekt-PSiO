#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include "Enemy.h" 

inline void collision(sf::Vector2f& pos, float radius, const std::vector<std::vector<bool>>& gridWalls) {
    if (pos.x < radius) pos.x = radius;
    if (pos.x > 800.f - radius) pos.x = 800.f - radius;
    if (pos.y < radius) pos.y = radius;
    if (pos.y > 600.f - radius) pos.y = 600.f - radius;

    int minX = std::max(0, static_cast<int>(pos.x - radius) / TILE_SIZE);
    int maxX = std::min(GRID_W - 1, static_cast<int>(pos.x + radius) / TILE_SIZE);
    int minY = std::max(0, static_cast<int>(pos.y - radius) / TILE_SIZE);
    int maxY = std::min(GRID_H - 1, static_cast<int>(pos.y + radius) / TILE_SIZE);

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            if (gridWalls[y][x]) {
                float wallLeft = x * TILE_SIZE;
                float wallRight = wallLeft + TILE_SIZE;
                float wallTop = y * TILE_SIZE;
                float wallBottom = wallTop + TILE_SIZE;

                float closestX = std::max(wallLeft, std::min(pos.x, wallRight));
                float closestY = std::max(wallTop, std::min(pos.y, wallBottom));

                float distanceX = pos.x - closestX;
                float distanceY = pos.y - closestY;
                float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

                if (distanceSquared < (radius * radius)) {
                    float distance = std::sqrt(distanceSquared);
                    if (distance == 0.f) { distanceX = 1.f; distanceY = 0.f; distance = 1.f; }
                    float overlap = radius - distance;
                    pos.x += (distanceX / distance) * overlap;
                    pos.y += (distanceY / distance) * overlap;
                }
            }
        }
    }
}

inline void enemyCollisions(std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (size_t i = 0; i < enemies.size(); i++) {
        for (size_t j = i + 1; j < enemies.size(); j++) {
            sf::Vector2f diff = enemies[i]->pos - enemies[j]->pos;
            float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            float minDist = enemies[i]->shape.getRadius() + enemies[j]->shape.getRadius();

            if (dist < minDist) {
                if (dist == 0.f) { diff = sf::Vector2f(1.f, 0.f); dist = 1.f; }
                float overlap = minDist - dist;
                sf::Vector2f push = (diff / dist) * (overlap * 0.5f);
                enemies[i]->pos += push;
                enemies[j]->pos -= push;
            }
        }
    }
}

inline void playerEnemyCollisions(sf::Vector2f& playerPos, float playerRadius, std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (auto& e : enemies) {
        sf::Vector2f diff = playerPos - e->pos;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        float minDist = playerRadius + e->shape.getRadius();

        if (dist < minDist) {
            if (dist == 0.f) {
                diff = sf::Vector2f(1.f, 0.f);
                dist = 1.f;
            }
            float overlap = minDist - dist;
            sf::Vector2f push = (diff / dist) * (overlap * 0.5f);
            playerPos += push;
            e->pos -= push;
        }
    }
}