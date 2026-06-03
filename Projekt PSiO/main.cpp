#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Splash.h"

// Funkcja zapobiegająca wychodzeniu poza mapę i wchodzeniu w ściany
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

// DODANE: Funkcja odpychająca wrogów od siebie (Anti-Entity Cramping / Separation)
void resolveEnemyCollisions(std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (size_t i = 0; i < enemies.size(); ++i) {
        for (size_t j = i + 1; j < enemies.size(); ++j) {
            sf::Vector2f diff = enemies[i]->pos - enemies[j]->pos;
            float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
               
            // Każdy wróg ma promień 14.f, więc minimalny bezpieczny dystans to 28.f
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

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Glowne Okno Gry");
    window.setFramerateLimit(60);

    Player player;
    player.pos = sf::Vector2f(400.f, 500.f);
    player.hp = 100.f;
    player.meleeCooldown = 0.f;

    std::vector<Bullet> bullets;
    std::vector<Splash> splashes;
    std::vector<std::unique_ptr<Enemy>> enemies;

    // Tworzymy trójkę początkowych wrogów
    enemies.push_back(std::make_unique<MeleeEnemy>(sf::Vector2f(200.f, 100.f)));
    enemies.push_back(std::make_unique<ThrowerEnemy>(sf::Vector2f(400.f, 100.f)));
    enemies.push_back(std::make_unique<ShooterEnemy>(sf::Vector2f(600.f, 100.f)));

    std::vector<std::vector<bool>> gridWalls(GRID_H, std::vector<bool>(GRID_W, false));
    std::vector<sf::RectangleShape> visualWalls;

    // Tworzenie "muru" na środku ekranu
    int wallY = 7;
    for (int x = 5; x <= 15; ++x) {
        gridWalls[wallY][x] = true;
        sf::RectangleShape wallRect(sf::Vector2f(static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE)));
        wallRect.setPosition(static_cast<float>(x * TILE_SIZE), static_cast<float>(wallY * TILE_SIZE));
        wallRect.setFillColor(sf::Color(100, 100, 100));
        visualWalls.push_back(wallRect);
    }

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // ==========================================
        // 1. LOGIKA GRACZA
        // ==========================================
        // Przekazujemy wrogów, żeby gracz mógł uderzyć ich w swojej własnej klasie
        player.update(dt, window, bullets, splashes, enemies);
        resolveCollision(player.pos, 14.f, gridWalls);

        if (player.hp <= 0.f) {
            player.hp = 100.f;
            player.pos = sf::Vector2f(400.f, 500.f);
        }

        // ==========================================
        // 2. LOGIKA INNYCH OBIEKTÓW
        // ==========================================
        for (auto& e : enemies) {
            e->update(dt, player.pos, player.hp, gridWalls, bullets, splashes);
            resolveCollision(e->pos, 14.f, gridWalls); // Nie mogą wejść w ścianę
        }

        // Odepchnięcie wrogów od siebie NA SAMYM KOŃCU ruchu
        resolveEnemyCollisions(enemies);

        // Kiedy pozycje są już ostatecznie wyliczone, przypisujemy je do kółek (hitboxów)
        for (auto& e : enemies) {
            e->shape.setPosition(e->pos);
        }

        for (auto& b : bullets) {
            b.update(dt, visualWalls);
        }

        for (auto& s : splashes) {
            s.update(dt);
        }

        // ==========================================
        // 3. GARBAGE COLLECTION
        // ==========================================
        for (auto& b : bullets) {
            if (b.lifetime <= 0.f && b.isSplashProjectile) {
                splashes.emplace_back(b.pos, b.isFireSplash);
            }
        }

        // Czyszczenie martwych pocisków, plam i wrogów z wektorów
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) {
            return b.lifetime <= 0.f;
            }), bullets.end());

        splashes.erase(std::remove_if(splashes.begin(), splashes.end(), [](const Splash& s) {
            return s.lifetime <= 0.f;
            }), splashes.end());

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::unique_ptr<Enemy>& e) {
            return e->hp <= 0.f;
            }), enemies.end());

        // ==========================================
        // 4. RENDERING
        // ==========================================
        window.clear(sf::Color(40, 40, 40));

        for (const auto& wall : visualWalls) window.draw(wall);
        for (auto& s : splashes) s.draw(window);
        for (auto& e : enemies) e->draw(window);
        for (auto& b : bullets) b.draw(window);
        player.draw(window);

        window.display();
    }

    return 0;
}