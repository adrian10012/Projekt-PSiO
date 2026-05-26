#include "Enemy.h"
#include <cmath>
#include <queue>

const int TILE_SIZE = 40;
const int GRID_W = 20;
const int GRID_H = 15;

Enemy::Enemy(sf::Vector2f startPos, sf::Color color, float s) {
    pos = startPos;
    speed = s;
    shape.setRadius(14.f);
    shape.setOrigin(14.f, 14.f);
    shape.setFillColor(color);
}

void Enemy::update(float dt, sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) {
    sf::Vector2f finalTarget;
    if (!Sees(pos, playerPos, gridWalls)) {
        finalTarget = Dijkstra(playerPos, gridWalls);
    }
    else {
        finalTarget = idealTarget(playerPos, gridWalls);
    }

    sf::Vector2f moveDir = finalTarget - pos;
    float dist = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
    if (dist > 0.1f) {
        moveDir.x /= dist;
        moveDir.y /= dist;
        pos.x += moveDir.x * speed * dt;
        pos.y += moveDir.y * speed * dt;
    }
    shape.setPosition(pos);
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Enemy::Sees(sf::Vector2f start, sf::Vector2f target, const std::vector<std::vector<bool>>& gridWalls) {
    sf::Vector2f dir = target - start;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist == 0.f) return true;

    int steps = static_cast<int>(dist / 10.f);
    for (int i = 0; i <= steps; ++i) {
        sf::Vector2f checkPos = start + (dir * (static_cast<float>(i) / static_cast<float>(steps)));
        int gridX = static_cast<int>(checkPos.x) / TILE_SIZE;
        int gridY = static_cast<int>(checkPos.y) / TILE_SIZE;

        if (gridX >= 0 && gridX < GRID_W && gridY >= 0 && gridY < GRID_H) {
            if (gridWalls[gridY][gridX]) return false;
        }
        else {
            return false;
        }
    }
    return true;
}

sf::Vector2f Enemy::Dijkstra(sf::Vector2f targetPos, const std::vector<std::vector<bool>>& gridWalls) {
    int startX = static_cast<int>(pos.x) / TILE_SIZE;
    int startY = static_cast<int>(pos.y) / TILE_SIZE;
    int targetX = static_cast<int>(targetPos.x) / TILE_SIZE;
    int targetY = static_cast<int>(targetPos.y) / TILE_SIZE;

    if (startX < 0) startX = 0; if (startX >= GRID_W) startX = GRID_W - 1;
    if (startY < 0) startY = 0; if (startY >= GRID_H) startY = GRID_H - 1;
    if (targetX < 0) targetX = 0; if (targetX >= GRID_W) targetX = GRID_W - 1;
    if (targetY < 0) targetY = 0; if (targetY >= GRID_H) targetY = GRID_H - 1;

    std::vector<std::vector<int>> dist(GRID_H, std::vector<int>(GRID_W, 9999));
    std::vector<std::vector<sf::Vector2i>> prev(GRID_H, std::vector<sf::Vector2i>(GRID_W, sf::Vector2i(-1, -1)));
    std::vector<sf::Vector2i> Visit;
    int i = 0;

    dist[startY][startX] = 0;
    Visit.push_back(sf::Vector2i(startX, startY));
    sf::Vector2i directions[] = { sf::Vector2i(0, -1), sf::Vector2i(0, 1), sf::Vector2i(-1, 0), sf::Vector2i(1, 0) };

    while (i < Visit.size()) {
        sf::Vector2i curr = Visit[i];
        i++;
        if (curr.x == targetX && curr.y == targetY) { break; }
        for (const auto& d : directions) {
            int dx = curr.x + d.x;
            int dy = curr.y + d.y;

            if (dx >= 0 && dx < GRID_W && dy >= 0 && dy < GRID_H) {
                if (!gridWalls[dy][dx] && dist[curr.y][curr.x] + 1 < dist[dy][dx]) {
                    dist[dy][dx] = dist[curr.y][curr.x] + 1;
                    prev[dy][dx] = curr;
                    Visit.push_back(sf::Vector2i(dx, dy));
                }
            }
        }
    }
    sf::Vector2i end(targetX, targetY);
    sf::Vector2i start(startX, startY);

    while (prev[end.y][end.x] != start && prev[end.y][end.x].x != -1) {
        end = prev[end.y][end.x];
    }
    float halfTile = static_cast<float>(TILE_SIZE) / 2.f;
    float nextX = static_cast<float>(end.x) * static_cast<float>(TILE_SIZE) + halfTile;
    float nextY = static_cast<float>(end.y) * static_cast<float>(TILE_SIZE) + halfTile;

    return sf::Vector2f(nextX, nextY);
}

MeleeEnemy::MeleeEnemy(sf::Vector2f startPos) : Enemy(startPos, sf::Color::Red, 120.f) {}

sf::Vector2f MeleeEnemy::idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) {
    sf::Vector2f dir = playerPos - pos;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist > 0.f) { dir.x /= dist; dir.y /= dist; }
    return pos + (dir * 500.f);
}

ThrowerEnemy::ThrowerEnemy(sf::Vector2f startPos) : Enemy(startPos, sf::Color::Green, 90.f) {}

sf::Vector2f ThrowerEnemy::idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) {
    sf::Vector2f dir = playerPos - pos;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist > 0.f) { dir.x /= dist; dir.y /= dist; }
    if (dist < 130.f) state = 0;
    else if (dist > 170.f) state = 1;

    if (state == 0) return pos - (dir * 500.f);
    return pos + (dir * 500.f);
}

ShooterEnemy::ShooterEnemy(sf::Vector2f startPos) : Enemy(startPos, sf::Color::Magenta, 100.f) {}

sf::Vector2f ShooterEnemy::idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) {
    sf::Vector2f dir = playerPos - pos;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist > 0.f) { dir.x /= dist; dir.y /= dist; }
    sf::Vector2f moveDir(-dir.y * strafeSign, dir.x * strafeSign);
    if (dist > 200.f) moveDir += dir;
    else if (dist < 120.f) moveDir -= dir;

    float mDist = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
    if (mDist > 0.f) { moveDir.x /= mDist; moveDir.y /= mDist; }
    sf::Vector2f checkPoint = pos + (moveDir * 30.f);

    if (!Sees(pos, checkPoint, gridWalls)) {
        strafeSign *= -1;
        moveDir = sf::Vector2f(-dir.y * strafeSign, dir.x * strafeSign);
        if (dist > 200.f) moveDir += dir;
        else if (dist < 120.f) moveDir -= dir;

        mDist = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        if (mDist > 0.f) { moveDir.x /= mDist; moveDir.y /= mDist; }
    }
    return pos + (moveDir * 500.f);
}