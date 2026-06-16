#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Bullet.h"
#include "Splash.h"
#include "Inventory.h"

class Enemy;

class Player {
private:
    sf::Vector2f pos;
    float speed;
    sf::CircleShape shape;
    float attackCooldown = 0.f;
    float hp = 100.f;
    float maxHp = 100.f;
    float meleeCooldown = 0.f;
    float swordAnimTimer = 0.f;
    float swordAttackAngle = 0.f;
    sf::RectangleShape swordShape;

    sf::Sprite sprite;
    sf::Texture texRun;
    sf::Texture texShoot;
    sf::Texture swordTex;

    int currentFrame = 0;
    float frameDurationRun = 0.15f;
    float frameDurationShoot = 0.05f;
    float frameTimer = 0.f;

    bool isMoving = false;
    bool isShooting = false;
    bool facingRight = true;

    int runFrameWidth;
    int runFrameHeight;
    int shootFrameWidth;
    int shootFrameHeight;

public:
    Inventory inventory;
    float baseDamage = 10.f;
    float finalDamage = 10.f;
    float baseArmor = 0.f;
    float finalArmor = 0.f;

    Player();

    sf::Vector2f getPos() const;
    float getSpeed() const;
    float getHp() const;
    sf::CircleShape getShape() const;

    void setPos(sf::Vector2f newPos);
    void setHp(float newHp);

    void update(float dt, sf::RenderWindow& window, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes, std::vector<std::unique_ptr<Enemy>>& enemies);
    void draw(sf::RenderWindow& window);
};