#include "Player.h"
#include "Enemy.h"
#include <cmath>
#include <algorithm>

Player::Player() {
    shape.setRadius(16.f);
    shape.setOrigin(16.f, 16.f);
    shape.setFillColor(sf::Color::Transparent);
    pos = sf::Vector2f(400.f, 300.f);
    speed = 250.f;

    swordShape.setSize(sf::Vector2f(40.f, 6.f));
    swordShape.setOrigin(0.f, 3.f);
    swordShape.setFillColor(sf::Color(200, 200, 200));

    texRun.loadFromFile("textures/Archer_Run.png");
    texShoot.loadFromFile("textures/Archer_Shoot.png");
    swordTex.loadFromFile("textures/Swords.png");

    sprite.setTexture(texRun);

    runFrameWidth = texRun.getSize().x / 4;
    runFrameHeight = texRun.getSize().y;

    shootFrameWidth = texShoot.getSize().x / 8;
    shootFrameHeight = texShoot.getSize().y;

    sprite.setOrigin(runFrameWidth / 2.f, runFrameHeight / 2.f);
}

sf::Vector2f Player::getPos() const { return pos; }
float Player::getSpeed() const { return speed; }
float Player::getHp() const { return hp; }
sf::CircleShape Player::getShape() const { return shape; }

void Player::setPos(sf::Vector2f newPos) {
    pos = newPos;
    shape.setPosition(pos);
}
void Player::setHp(float newHp) { hp = newHp; }

void Player::update(float dt, sf::RenderWindow& window, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes, std::vector<std::unique_ptr<Enemy>>& enemies) {
    finalDamage = baseDamage;
    finalArmor = baseArmor;

    if (inventory.bron) finalDamage += inventory.bron->get_damage();
    if (inventory.helm) finalArmor += inventory.helm->get_damage();
    if (inventory.klata) finalArmor += inventory.klata->get_damage();
    if (inventory.spodnie) finalArmor += inventory.spodnie->get_damage();
    if (inventory.buty) finalArmor += inventory.buty->get_damage();

    isMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { pos.y -= speed * dt; isMoving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { pos.y += speed * dt; isMoving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { pos.x -= speed * dt; isMoving = true; if (!isShooting) facingRight = false; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { pos.x += speed * dt; isMoving = true; if (!isShooting) facingRight = true; }

    shape.setPosition(pos);
    sprite.setPosition(pos);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && attackCooldown <= 0.f) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f aimDir = sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) - pos;
        float aimLen = std::sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y);
        if (aimLen > 0) { aimDir.x /= aimLen; aimDir.y /= aimLen; }

        bullets.emplace_back(pos, aimDir, 25.f, false, false, false, 1000.f, 4.f);
        attackCooldown = 0.25f;

        isShooting = true;
        currentFrame = 0;
        frameTimer = 0.f;
    }

    if (isShooting) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        facingRight = (static_cast<float>(mousePos.x) >= pos.x);

        frameTimer += dt;
        if (frameTimer >= frameDurationShoot) {
            frameTimer = 0.f;
            currentFrame++;
            if (currentFrame >= 8) {
                isShooting = false;
                currentFrame = 0;
            }
        }

        sprite.setTexture(texShoot);
        sprite.setTextureRect(sf::IntRect(currentFrame * shootFrameWidth, 0, shootFrameWidth, shootFrameHeight));
        sprite.setOrigin(shootFrameWidth / 2.f, shootFrameHeight / 2.f);
    }
    else {
        if (isMoving) {
            frameTimer += dt;
            if (frameTimer >= frameDurationRun) {
                frameTimer = 0.f;
                currentFrame = (currentFrame + 1) % 4;
            }
        }
        else {
            currentFrame = 0;
        }

        sprite.setTexture(texRun);
        sprite.setTextureRect(sf::IntRect(currentFrame * runFrameWidth, 0, runFrameWidth, runFrameHeight));
        sprite.setOrigin(runFrameWidth / 2.f, runFrameHeight / 2.f);
    }

    if (facingRight) {
        sprite.setScale(1.f, 1.f);
    }
    else {
        sprite.setScale(-1.f, 1.f);
    }

    if (attackCooldown > 0.f) attackCooldown -= dt;
    if (swordAnimTimer > 0.f) swordAnimTimer -= dt;
    if (meleeCooldown > 0.f) meleeCooldown -= dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && meleeCooldown <= 0.f) {
        swordAnimTimer = 0.30f;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f aimDir = sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) - pos;
        swordAttackAngle = std::atan2(aimDir.y, aimDir.x) * 180.f / 3.14159f;

        for (auto& e : enemies) {
            sf::Vector2f dir = e->getPos() - pos;
            float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (dist < 60.f) {
                float enemyAngle = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
                float angleDiff = std::abs(enemyAngle - swordAttackAngle);
                if (angleDiff > 180.f) angleDiff = 360.f - angleDiff;

                if (angleDiff <= 45.f) {
                    if (!e->getIsInvulnerable()) {
                        e->setHp(e->getHp() - finalDamage);
                    }
                }
            }
        }
        meleeCooldown = 0.5f;
    }

    for (auto& b : bullets) {
        if (b.getLifetime() <= 0.f || !b.getIsEnemy() || b.getIsSplash()) continue;

        if (shape.getGlobalBounds().intersects(b.getShape().getGlobalBounds())) {
            float damageTaken = std::max(1.f, b.getDamage() - finalArmor);
            hp -= damageTaken;
            b.setLifetime(0.f);
        }
    }

    for (const auto& s : splashes) {
        sf::Vector2f sPos = s.getPos();
        float dist = std::sqrt((pos.x - sPos.x) * (pos.x - sPos.x) + (pos.y - sPos.y) * (pos.y - sPos.y));

        if (dist < (shape.getRadius() + s.getCurrRadius())) {
            if (s.getIsBomb()) {
                hp -= 200.f * dt;
            }
            else {
                hp -= 15.f * dt;
            }
        }
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);

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
        float finalAngle = swordAttackAngle + swingAngle;

        if (inventory.bron && inventory.bron->get_typ() == "Mele") {
            int colorRow = 4;
            int length = 0;

            if (inventory.bron->get_nazwa() == "Slaby Miecz") { colorRow = 0; length = 1; }
            else if (inventory.bron->get_nazwa() == "Zwykly Miecz") { colorRow = 2; length = 2; }
            else if (inventory.bron->get_nazwa() == "Dobry Miecz") { colorRow = 1; length = 3; }
            else if (inventory.bron->get_nazwa() == "Wysmienity Miecz") { colorRow = 3; length = 4; }

            int partW = swordTex.getSize().x / 3;
            int partH = swordTex.getSize().y / 5;

            sf::Sprite s(swordTex);
            s.setOrigin(0, partH / 2.f);

            float scale = 22.f / static_cast<float>(partW);
            s.setScale(scale, scale);

            sf::Transform t;
            t.translate(pos);
            t.rotate(finalAngle);

            float advance = partW * scale * 0.38f;

            s.setTextureRect(sf::IntRect(0, colorRow * partH, partW, partH));
            window.draw(s, sf::RenderStates(t));
            t.translate(advance, 0);

            s.setTextureRect(sf::IntRect(partW, colorRow * partH, partW, partH));
            for (int i = 0; i < length; i++) {
                window.draw(s, sf::RenderStates(t));
                t.translate(advance, 0);
            }

            s.setTextureRect(sf::IntRect(2 * partW, colorRow * partH, partW, partH));
            window.draw(s, sf::RenderStates(t));
        }
        else {
            swordShape.setPosition(pos);
            swordShape.setRotation(finalAngle);
            window.draw(swordShape);
        }
    }
}