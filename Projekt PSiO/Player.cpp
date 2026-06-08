#include "Player.h"
#include "Enemy.h"
#include <cmath>
#include <algorithm>

Player::Player() {
    shape.setRadius(16.f);
    shape.setOrigin(16.f, 16.f);
    shape.setFillColor(sf::Color::Blue);
    pos = sf::Vector2f(400.f, 300.f);
    speed = 250.f;

    swordShape.setSize(sf::Vector2f(40.f, 6.f));
    swordShape.setOrigin(0.f, 3.f);
    swordShape.setFillColor(sf::Color(200, 200, 200));
}

void Player::update(float dt, sf::RenderWindow& window, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes, std::vector<std::unique_ptr<Enemy>>& enemies) {
    finalDamage = baseDamage;
    finalArmor = baseArmor;

    if (inventory.bron) finalDamage += inventory.bron->get_damage();
    if (inventory.helm) finalArmor += inventory.helm->get_damage();
    if (inventory.klata) finalArmor += inventory.klata->get_damage();
    if (inventory.spodnie) finalArmor += inventory.spodnie->get_damage();
    if (inventory.buty) finalArmor += inventory.buty->get_damage();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) pos.y -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) pos.y += speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) pos.x -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) pos.x += speed * dt;
    shape.setPosition(pos);

    if (attackCooldown > 0.f) attackCooldown -= dt;
    if (swordAnimTimer > 0.f) swordAnimTimer -= dt;
    if (meleeCooldown > 0.f) meleeCooldown -= dt;

    // Atak wrêcz
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && meleeCooldown <= 0.f) {
        swordAnimTimer = 0.30f;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f aimDir = sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) - pos;
        swordAttackAngle = std::atan2(aimDir.y, aimDir.x) * 180.f / 3.14159f;

        for (auto& e : enemies) {
            sf::Vector2f dir = e->pos - pos;
            float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (dist < 60.f) {
                float enemyAngle = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
                float angleDiff = std::abs(enemyAngle - swordAttackAngle);
                if (angleDiff > 180.f) angleDiff = 360.f - angleDiff;

                if (angleDiff <= 45.f) {
                    if (!e->isInvulnerable) {
                        e->hp -= finalDamage;
                    }
                }
            }
        }
        meleeCooldown = 0.5f;
    }

    // Strzelanie
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && attackCooldown <= 0.f) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f aimDir = sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) - pos;
        float aimLen = std::sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y);
        if (aimLen > 0) { aimDir.x /= aimLen; aimDir.y /= aimLen; }

        bullets.emplace_back(pos, aimDir, 25.f, false, false, false, 1000.f, 4.f);
        attackCooldown = 0.25f;
    }

    // Obra¿enia pociski
    for (auto& b : bullets) {
        if (b.lifetime <= 0.f || !b.isEnemy || b.isSplashProjectile) continue;
        if (shape.getGlobalBounds().intersects(b.shape.getGlobalBounds())) {
            float damageTaken = std::max(1.f, b.damage - finalArmor);
            hp -= damageTaken;
            b.lifetime = 0.f;
        }
    }

    // Obra¿enia splash
    for (const auto& s : splashes) {
        if (shape.getGlobalBounds().intersects(s.shape.getGlobalBounds())) {
            if (s.isFire) {
                hp -= 200.f * dt;
            }
            else {
                hp -= 15.f * dt;
            }
        }
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);

    sf::RectangleShape hpBg(sf::Vector2f(40.f, 5.f));
    hpBg.setFillColor(sf::Color::Red);
    hpBg.setPosition(pos.x - 20.f, pos.y - shape.getRadius() - 15.f);
    sf::RectangleShape hpFg(sf::Vector2f(40.f * std::max(0.f, hp / maxHp), 5.f));
    hpFg.setFillColor(sf::Color::Green);
    hpFg.setPosition(pos.x - 20.f, pos.y - shape.getRadius() - 15.f);
    window.draw(hpBg);
    window.draw(hpFg);

    if (swordAnimTimer > 0.f) {
        float progress = 1.0f - (swordAnimTimer / 0.3f);
        float swingAngle = -45.f + (progress * 90.f);
        swordShape.setPosition(pos);
        swordShape.setRotation(swordAttackAngle + swingAngle);
        window.draw(swordShape);
    }
}