#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Bullet.h"
#include "Splash.h"
#include "Collision.h"
#include "CombatManager.h"
#include "Menu.h"
#include "Ekwipunek.h"
#include "Platnerz.h"
#include "Wiedzma.h"
#include "Kowal.h"
#include "Inventory.h"

enum class GameState { MIASTO, WALKA };

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "window");
    window.setFramerateLimit(60);

    GameState currentState = GameState::MIASTO;
    sf::Font font;
    font.loadFromFile("OpenSans-SemiBold.ttf");

    Player player;
    Menu menu({ 150, 80 }, { 600, 400 }, font);
    Ekwipunek ekwipunek(font, &player);
    Platnerz platnerz(font, &player);
    Wiedzma wiedzma(font, &player);
    Kowal kowal(font, &player);
    player.setPos(sf::Vector2f(400.f, 530.f));
    player.setHp(100.f);

    std::vector<Bullet> bullets;
    std::vector<Splash> splashes;
    std::vector<std::unique_ptr<Enemy>> enemies;

    std::vector<std::vector<bool>> gridWalls(GRID_H, std::vector<bool>(GRID_W, false));
    std::vector<sf::RectangleShape> visualWalls;

    auto addWall = [&](int x, int y) {
        if (x >= 0 && x < GRID_W && y >= 0 && y < GRID_H) {
            gridWalls[y][x] = true;
            sf::RectangleShape wallRect(sf::Vector2f(static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE)));
            wallRect.setPosition(static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE));
            wallRect.setFillColor(sf::Color(80, 80, 80));
            wallRect.setOutlineThickness(-2.f);
            wallRect.setOutlineColor(sf::Color(50, 50, 50));
            visualWalls.push_back(wallRect);
        }
        };

    for (int x = 2; x <= 5; ++x) { addWall(x, 3); addWall(x, 11); }
    for (int x = 14; x <= 17; ++x) { addWall(x, 3); addWall(x, 11); }
    for (int y = 3; y <= 5; ++y) { addWall(2, y); addWall(17, y); }
    for (int y = 9; y <= 11; ++y) { addWall(2, y); addWall(17, y); }
    for (int x = 8; x <= 11; ++x) { addWall(x, 6); addWall(x, 8); }
    addWall(7, 7); addWall(12, 7);

    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Tab) {
                    currentState = (currentState == GameState::MIASTO) ? GameState::WALKA : GameState::MIASTO;
                }

                if (currentState == GameState::WALKA) {
                    if (event.key.code == sf::Keyboard::Num0) {
                        enemies.push_back(std::make_unique<FirstMeleeEnemy>(sf::Vector2f(60.f, 60.f)));
                        enemies.push_back(std::make_unique<SecondMeleeEnemy>(sf::Vector2f(740.f, 60.f)));
                        enemies.push_back(std::make_unique<FirstThrowerEnemy>(sf::Vector2f(60.f, 530.f)));
                        enemies.push_back(std::make_unique<SecondThrowerEnemy>(sf::Vector2f(740.f, 530.f)));
                        enemies.push_back(std::make_unique<FirstShooterEnemy>(sf::Vector2f(400.f, 100.f)));
                        enemies.push_back(std::make_unique<SecondShooterEnemy>(sf::Vector2f(400.f, 400.f)));
                    }
                    if (event.key.code == sf::Keyboard::Num1) {
                        enemies.push_back(std::make_unique<BossFirst>(sf::Vector2f(400.f, 150.f)));
                    }
                    if (event.key.code == sf::Keyboard::Num2) {
                        enemies.push_back(std::make_unique<BossSecond>(sf::Vector2f(400.f, 150.f), 3));
                    }
                    if (event.key.code == sf::Keyboard::Num3) {
                        enemies.push_back(std::make_unique<BossThird>(sf::Vector2f(400.f, 150.f)));
                    }
                    if (event.key.code == sf::Keyboard::M) {
                        player.baseDamage *= 5.0f;
                    }
                }
            }

            if (currentState == GameState::MIASTO) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mouse = sf::Mouse::getPosition(window);
                    if (ekwipunek.isOpen()) ekwipunek.handleClick(mouse);
                    else if (platnerz.isOpen()) platnerz.handleClick(mouse);
                    else if (wiedzma.isOpen()) wiedzma.handleClick(mouse);
                    else if (kowal.isOpen()) kowal.handleClick(mouse);
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

            sf::Vector2f pPos = player.getPos();
            collision(pPos, 14.f, gridWalls);
            player.setPos(pPos);

            if (player.getHp() <= 0.f) {
                player.setHp(100.f);
                player.setPos(sf::Vector2f(400.f, 530.f));
            }

            std::vector<std::unique_ptr<Enemy>> newSpawns;

            for (auto& e : enemies) {
                float currentHp = player.getHp();
                e->update(dt, player.getPos(), currentHp, gridWalls, bullets, splashes);
                player.setHp(currentHp);
                sf::Vector2f ePos = e->getPos();
                collision(ePos, e->getShape().getRadius(), gridWalls);
                e->setPos(ePos);
            }

            enemyCollisions(enemies);

            sf::Vector2f playerCollisionPos = player.getPos();
            playerEnemyCollisions(playerCollisionPos, 16.f, enemies);
            player.setPos(playerCollisionPos);
            for (auto& b : bullets) b.update(dt, visualWalls);
            for (auto& s : splashes) s.update(dt);
            cleanupCombat(enemies, bullets, splashes, newSpawns);
            for (auto& newE : newSpawns) {
                enemies.push_back(std::move(newE));
            }
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