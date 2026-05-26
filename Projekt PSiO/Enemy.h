#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

extern const int TILE_SIZE;
extern const int GRID_W;
extern const int GRID_H;

class Enemy {
public:
    sf::Vector2f pos;
    sf::CircleShape shape;
    float speed;

    Enemy(sf::Vector2f startPos, sf::Color color, float s);
    virtual ~Enemy() {}

    void update(float dt, sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls);
    void draw(sf::RenderWindow& window);
protected:
    virtual sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) = 0;

    bool Sees(sf::Vector2f start, sf::Vector2f target, const std::vector<std::vector<bool>>& gridWalls);
    sf::Vector2f Dijkstra(sf::Vector2f targetPos, const std::vector<std::vector<bool>>& gridWalls);
};

class MeleeEnemy : public Enemy {
public:
    MeleeEnemy(sf::Vector2f startPos);
protected:
    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
};

class ThrowerEnemy : public Enemy {
private:
    int state = 1;
public:
    ThrowerEnemy(sf::Vector2f startPos);
protected:
    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
};

class ShooterEnemy : public Enemy {
private:
    int strafeSign = 1;
public:
    ShooterEnemy(sf::Vector2f startPos);
protected:
    sf::Vector2f idealTarget(sf::Vector2f playerPos, const std::vector<std::vector<bool>>& gridWalls) override;
};