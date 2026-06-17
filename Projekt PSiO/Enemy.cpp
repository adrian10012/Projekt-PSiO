#include "Enemy.h"
#include <cmath>
#include <cstdlib>

Enemy::Enemy(sf::Vector2f startPos, sf::Color color, float s, float cooldownTime, float maxHpValue, float radius) {
    pos = startPos;
    speed = s;
    hp = maxHpValue;
    maxHp = maxHpValue;
    attackResetTime = cooldownTime;
    attackCooldown = cooldownTime + static_cast<float>(rand() % 15) * 0.1f;

    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setFillColor(color);
}

sf::Vector2f Enemy::getPos() const { return pos; }
float Enemy::getHp() const { return hp; }
sf::CircleShape Enemy::getShape() const { return shape; }
bool Enemy::getIsInvulnerable() const { return isInvulnerable; }
void Enemy::setPos(sf::Vector2f newPos) { pos = newPos; shape.setPosition(pos); }
void Enemy::setHp(float newHp) { hp = newHp; }

void Enemy::update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) {
    if (attackCooldown > 0.f) attackCooldown -= dt;

    sf::Vector2f finalTarget;
    if (!Sees(pos, playerPos, gridWalls)) finalTarget = Dijkstra(playerPos, gridWalls);
    else finalTarget = idealTarget(playerPos, gridWalls);

    sf::Vector2f moveDir = finalTarget - pos;
    float dist = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
    if (dist > 0.1f) {
        moveDir.x /= dist; moveDir.y /= dist;
        pos.x += moveDir.x * speed * dt; pos.y += moveDir.y * speed * dt;
    }
    shape.setPosition(pos);

    for (auto& b : bullets) {
        if (b.getLifetime() <= 0.f || b.getIsEnemy() || b.getIsSplash()) continue;
        if (shape.getGlobalBounds().intersects(b.getShape().getGlobalBounds())) {
            if (!isInvulnerable) hp -= b.getDamage();
            b.setLifetime(0.f);
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
    sf::RectangleShape hpBg(sf::Vector2f(30, 4)); hpBg.setFillColor(sf::Color::Red); hpBg.setPosition(pos.x - 15, pos.y - shape.getRadius() - 10);
    sf::RectangleShape hpFg(sf::Vector2f(30 * std::max(0.f, hp / maxHp), 4)); hpFg.setFillColor(sf::Color::Green); hpFg.setPosition(pos.x - 15, pos.y - shape.getRadius() - 10);
    window.draw(hpBg); window.draw(hpFg);
}

bool Enemy::Sees(sf::Vector2f start, sf::Vector2f target, const std::vector<std::vector<bool>>& gridWalls) {
    sf::Vector2f dir = target - start;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist == 0.f) return true;
    int steps = static_cast<int>(dist / 10.f);
    for (int i = 0; i <= steps; i++) {
        sf::Vector2f checkPos = start + (dir * (static_cast<float>(i) / static_cast<float>(steps)));
        int gridX = static_cast<int>(checkPos.x) / TILE_SIZE; int gridY = static_cast<int>(checkPos.y) / TILE_SIZE;
        if (gridX >= 0 && gridX < GRID_W && gridY >= 0 && gridY < GRID_H) {
            if (gridWalls[gridY][gridX]) return false;
        }
        else return false;
    }
    return true;
}

sf::Vector2f Enemy::Dijkstra(sf::Vector2f targetPos, const std::vector<std::vector<bool>>& gridWalls) {
    int startX = static_cast<int>(pos.x) / TILE_SIZE; int startY = static_cast<int>(pos.y) / TILE_SIZE;
    int targetX = static_cast<int>(targetPos.x) / TILE_SIZE; int targetY = static_cast<int>(targetPos.y) / TILE_SIZE;
    if (startX < 0) startX = 0; if (startX >= GRID_W) startX = GRID_W - 1;
    if (startY < 0) startY = 0; if (startY >= GRID_H) startY = GRID_H - 1;
    if (targetX < 0) targetX = 0; if (targetX >= GRID_W) targetX = GRID_W - 1;
    if (targetY < 0) targetY = 0; if (targetY >= GRID_H) targetY = GRID_H - 1;

    std::vector<std::vector<int>> dist(GRID_H, std::vector<int>(GRID_W, 9999));
    std::vector<std::vector<sf::Vector2i>> prev(GRID_H, std::vector<sf::Vector2i>(GRID_W, sf::Vector2i(-1, -1)));
    std::vector<sf::Vector2i> Visit;
    int i = 0; dist[startY][startX] = 0; Visit.push_back(sf::Vector2i(startX, startY));
    sf::Vector2i directions[] = { sf::Vector2i(0, -1), sf::Vector2i(0, 1), sf::Vector2i(-1, 0), sf::Vector2i(1, 0) };

    while (i < Visit.size()) {
        sf::Vector2i curr = Visit[i]; i++;
        if (curr.x == targetX && curr.y == targetY) { break; }
        for (const auto& d : directions) {
            int dx = curr.x + d.x; int dy = curr.y + d.y;
            if (dx >= 0 && dx < GRID_W && dy >= 0 && dy < GRID_H) {
                if (!gridWalls[dy][dx] && dist[curr.y][curr.x] + 1 < dist[dy][dx]) {
                    dist[dy][dx] = dist[curr.y][curr.x] + 1; prev[dy][dx] = curr; Visit.push_back(sf::Vector2i(dx, dy));
                }
            }
        }
    }
    sf::Vector2i end(targetX, targetY); sf::Vector2i start(startX, startY);
    while (prev[end.y][end.x] != start && prev[end.y][end.x].x != -1) { end = prev[end.y][end.x]; }
    float halfTile = static_cast<float>(TILE_SIZE) / 2.f;
    return sf::Vector2f(static_cast<float>(end.x) * TILE_SIZE + halfTile, static_cast<float>(end.y) * TILE_SIZE + halfTile);
}

MeleeEnemy::MeleeEnemy(sf::Vector2f startPos, sf::Color color, float maxHp, float s, float radius, float dmg, float cd)
    : Enemy(startPos, color, s, cd, maxHp, radius), damage(dmg) {
    swordShape.setSize(sf::Vector2f(radius * 2.f, radius / 2.f));
    swordShape.setOrigin(0.f, radius / 4.f);
    swordShape.setFillColor(sf::Color(255, 150, 150));
}

sf::Vector2f MeleeEnemy::idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) {
    sf::Vector2f dir = playerPos - pos; float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist > 0.f) { dir.x /= dist; dir.y /= dist; }
    return pos + (dir * 500.f);
}

void MeleeEnemy::update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) {
    Enemy::update(dt, playerPos, playerHp, gridWalls, bullets, splashes);
    if (swordAnimTimer > 0.f) swordAnimTimer -= dt;
    sf::Vector2f dir = playerPos - pos; float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (dist < (shape.getRadius() + 20.f) && attackCooldown <= 0.f) {
        playerHp -= damage; attackCooldown = attackResetTime; swordAnimTimer = 0.15f;
        attackAngle = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
    }
}

void MeleeEnemy::draw(sf::RenderWindow& window) {
    Enemy::draw(window);
    if (swordAnimTimer > 0.f) {
        float progress = 1.0f - (swordAnimTimer / 0.15f); float swingAngle = -45.f + (progress * 90.f);
        swordShape.setPosition(pos); swordShape.setRotation(attackAngle + swingAngle); window.draw(swordShape);
    }
}

ThrowerEnemy::ThrowerEnemy(sf::Vector2f startPos, sf::Color color, float maxHp, float s, float radius, bool bomb, float cd)
    : Enemy(startPos, color, s, cd, maxHp, radius), throwsBomb(bomb) {
}

sf::Vector2f ThrowerEnemy::idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) {
    sf::Vector2f dir = playerPos - pos; float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist > 0.f) { dir.x /= dist; dir.y /= dist; }
    if (dist < 130.f) state = 0; else if (dist > 170.f) state = 1;
    if (state == 0) return pos - (dir * 500.f);
    return pos + (dir * 500.f);
}

void ThrowerEnemy::update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) {
    Enemy::update(dt, playerPos, playerHp, gridWalls, bullets, splashes);
    if (attackCooldown <= 0.f) {
        sf::Vector2f dir = playerPos - pos; float d = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (d > 0) { dir.x /= d; dir.y /= d; }
        float requiredSpeed = d / 0.6f;
        bullets.emplace_back(pos, dir, 15.f, true, true, throwsBomb, requiredSpeed, 6.f);
        attackCooldown = attackResetTime;
    }
}

ShooterEnemy::ShooterEnemy(sf::Vector2f startPos, sf::Color color, float maxHp, float s, float radius, float bSpeed, float bDmg, float cd)
    : Enemy(startPos, color, s, cd, maxHp, radius), bulletSpeed(bSpeed), bulletDamage(bDmg) {
}

sf::Vector2f ShooterEnemy::idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) {
    sf::Vector2f dir = playerPos - pos; float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist > 0.f) { dir.x /= dist; dir.y /= dist; }
    sf::Vector2f moveDir(-dir.y * strafeSign, dir.x * strafeSign);
    if (dist > 200.f) moveDir += dir; else if (dist < 120.f) moveDir -= dir;

    float moveDirLength = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
    if (moveDirLength > 0.f) { moveDir.x /= moveDirLength; moveDir.y /= moveDirLength; }

    sf::Vector2f checkPoint = pos + (moveDir * 30.f);
    if (!Sees(pos, checkPoint, gridWalls)) {
        strafeSign *= -1; moveDir = sf::Vector2f(-dir.y * strafeSign, dir.x * strafeSign);
        if (dist > 200.f) moveDir += dir; else if (dist < 120.f) moveDir -= dir;
        moveDirLength = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        if (moveDirLength > 0.f) { moveDir.x /= moveDirLength; moveDir.y /= moveDirLength; }
    }
    return pos + (moveDir * 500.f);
}

void ShooterEnemy::update(float dt, sf::Vector2f playerPos, float& playerHp, const std::vector<std::vector<bool>>& gridWalls, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes) {
    Enemy::update(dt, playerPos, playerHp, gridWalls, bullets, splashes);
    if (attackCooldown <= 0.f && Sees(pos, playerPos, gridWalls)) {
        sf::Vector2f dir = playerPos - pos; float d = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (d > 0) { dir.x /= d; dir.y /= d; }
        bullets.emplace_back(pos, dir, bulletDamage, true, false, false, bulletSpeed, 4.f);
        attackCooldown = attackResetTime;
    }
}

FirstMeleeEnemy::FirstMeleeEnemy(sf::Vector2f startPos) : MeleeEnemy(startPos, sf::Color(139, 0, 0), 200.f, 80.f, 20.f, 40.f, 2.0f) {}
SecondMeleeEnemy::SecondMeleeEnemy(sf::Vector2f startPos) : MeleeEnemy(startPos, sf::Color::Magenta, 60.f, 160.f, 10.f, 10.f, 0.5f) {}
FirstThrowerEnemy::FirstThrowerEnemy(sf::Vector2f startPos) : ThrowerEnemy(startPos, sf::Color(255, 140, 0), 120.f, 70.f, 14.f, true, 3.0f) {}
SecondThrowerEnemy::SecondThrowerEnemy(sf::Vector2f startPos) : ThrowerEnemy(startPos, sf::Color(0, 100, 0), 80.f, 110.f, 12.f, false, 1.5f) {}
FirstShooterEnemy::FirstShooterEnemy(sf::Vector2f startPos) : ShooterEnemy(startPos, sf::Color(128, 0, 128), 70.f, 60.f, 14.f, 1200.f, 30.f, 2.5f) {}
SecondShooterEnemy::SecondShooterEnemy(sf::Vector2f startPos) : ShooterEnemy(startPos, sf::Color(0, 206, 209), 100.f, 90.f, 14.f, 450.f, 5.f, 0.4f) {}