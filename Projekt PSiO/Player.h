#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Bullet.h"
#include "Splash.h"

class Enemy;

class Player {
public:
    sf::Vector2f pos;
    float speed;
    sf::CircleShape shape;
    float attackCooldown = 0.f;
    float hp = 100.f;
    float meleeCooldown = 0.f;
    float swordAnimTimer = 0.f;
    float swordAttackAngle = 0.f;
    sf::RectangleShape swordShape;

    Player();
    void update(float dt, sf::RenderWindow& window, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes, std::vector<std::unique_ptr<Enemy>>& enemies);

    void draw(sf::RenderWindow& window);
};