#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Splash.h"
#include "Menu.h"
#include "Ekwipunek.h"
#include "Platnerz.h"
#include "Wiedzma.h"
#include "Kowal.h"
#include "Bronie.h"
#include "Mikstura.h"
#include "Pancerze.h"
#include "Inventory.h"

enum class GameState {
    MIASTO,
    WALKA 
};

void resolveCollision(sf::Vector2f& pos, float radius, const std::vector<std::vector<bool>>& gridWalls) {
    if (pos.x < radius) pos.x = radius;
    if (pos.x > 800.f - radius) pos.x = 800.f - radius;
    if (pos.y < radius) pos.y = radius;
    if (pos.y > 600.f - radius) pos.y = 600.f - radius;

    int minX = static_cast<int>(pos.x - radius) / TILE_SIZE;
    int maxX = static_cast<int>(pos.x + radius) / TILE_SIZE;
    int minY = static_cast<int>(pos.y - radius) / TILE_SIZE;
    int maxY = static_cast<int>(pos.y + radius) / TILE_SIZE;
}

void resolveEnemyCollisions(std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (size_t i = 0; i < enemies.size(); ++i) {
        for (size_t j = i + 1; j < enemies.size(); ++j) {
            sf::Vector2f diff = enemies[i]->pos - enemies[j]->pos;
            float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            float minDist = 28.f;

            if (dist < minDist) {
                if (dist == 0.f) {
                    diff = sf::Vector2f(1.f, 0.f);
                    dist = 1.f;
                }
                float overlap = minDist - dist;
                sf::Vector2f push = (diff / dist) * (overlap * 0.5f);

                enemies[i]->pos += push;
                enemies[j]->pos -= push;
            }
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Jarek Kaczynski");
    window.setFramerateLimit(60);

    GameState currentState = GameState::MIASTO;
    sf::Font font;
    font.loadFromFile("OpenSans-SemiBold.ttf");

    Player player;
    Menu menu({ 150, 80 }, { 600, 400 }, font);
    Ekwipunek ekwipunek(font, &player);
    Platnerz platnerz(font, &player);
    Wiedzma wiedzma(font);
    Kowal kowal(font);

    player.pos = sf::Vector2f(400.f, 500.f);
    player.hp = 100.f;
    player.meleeCooldown = 0.f;

    std::vector<Bullet> bullets;
    std::vector<Splash> splashes;
    std::vector<std::unique_ptr<Enemy>> enemies;

    enemies.push_back(std::make_unique<MeleeEnemy>(sf::Vector2f(200.f, 100.f)));
    enemies.push_back(std::make_unique<ThrowerEnemy>(sf::Vector2f(400.f, 100.f)));
    enemies.push_back(std::make_unique<ShooterEnemy>(sf::Vector2f(600.f, 100.f)));

    std::vector<std::vector<bool>> gridWalls(GRID_H, std::vector<bool>(GRID_W, false));
    std::vector<sf::RectangleShape> visualWalls;

    int wallY = 7;
    for (int x = 5; x <= 15; ++x) {
        gridWalls[wallY][x] = true;
        sf::RectangleShape wallRect(sf::Vector2f(static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE)));
        wallRect.setPosition(static_cast<float>(x * TILE_SIZE), static_cast<float>(wallY * TILE_SIZE));
        wallRect.setFillColor(sf::Color(100, 100, 100));
        visualWalls.push_back(wallRect);
    }

    sf::Clock clock;

    while (window.isOpen())
    {
        
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
                if (currentState == GameState::MIASTO) currentState = GameState::WALKA;
                else currentState = GameState::MIASTO;
            }

            if (currentState == GameState::MIASTO)
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    sf::Vector2i mouse = sf::Mouse::getPosition(window);

                    if (ekwipunek.isOpen()) {
                        ekwipunek.handleClick(mouse);
                    }
                    else if (platnerz.isOpen()) {
                        platnerz.handleClick(mouse);
                    }
                    else if (wiedzma.isOpen()) {
                        wiedzma.handleClick(mouse);
                    }
                    else if (kowal.isOpen()) {
                        kowal.handleClick(mouse);
                    }
                    else {
                        menu.handleClick(mouse);

                        if (menu.getLastClicked() == "Ekwipunek") ekwipunek.toggle();
                        if (menu.getLastClicked() == "Platnerz") platnerz.toggle();
                        if (menu.getLastClicked() == "Wiedzma") wiedzma.toggle();
                        if (menu.getLastClicked() == "Kowal") kowal.toggle();
                    }
                }
            }
        }

        if (currentState == GameState::WALKA)
        {
            player.update(dt, window, bullets, splashes, enemies);
            resolveCollision(player.pos, 14.f, gridWalls);

            if (player.hp <= 0.f) {
                player.hp = 100.f;
                player.pos = sf::Vector2f(400.f, 500.f);
            }

            for (auto& e : enemies) {
                e->update(dt, player.pos, player.hp, gridWalls, bullets, splashes);
                resolveCollision(e->pos, 14.f, gridWalls);
            }

            resolveEnemyCollisions(enemies);

            for (auto& e : enemies) e->shape.setPosition(e->pos);
            for (auto& b : bullets) b.update(dt, visualWalls);
            for (auto& s : splashes) s.update(dt);

            for (auto& b : bullets) {
                if (b.lifetime <= 0.f && b.isSplashProjectile) {
                    splashes.emplace_back(b.pos, b.isFireSplash);
                }
            }

            bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return b.lifetime <= 0.f; }), bullets.end());
            splashes.erase(std::remove_if(splashes.begin(), splashes.end(), [](const Splash& s) { return s.lifetime <= 0.f; }), splashes.end());
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::unique_ptr<Enemy>& e) { return e->hp <= 0.f; }), enemies.end());
        }

        if (currentState == GameState::MIASTO)
        {
            window.clear(sf::Color(30, 30, 30));
            if (menu.open()) menu.draw(window);
            if (ekwipunek.isOpen()) ekwipunek.draw(window);
            if (platnerz.isOpen()) platnerz.draw(window);
            if (wiedzma.isOpen()) wiedzma.draw(window);
            if (kowal.isOpen()) kowal.draw(window);
        }
        else if (currentState == GameState::WALKA)
        {
            window.clear(sf::Color(40, 40, 40));

            for (const auto& wall : visualWalls) window.draw(wall);
            for (auto& s : splashes) s.draw(window);
            for (auto& e : enemies) e->draw(window);
            for (auto& b : bullets) b.draw(window);
            player.draw(window);
        }

        window.display();
    }

    return 0;
}