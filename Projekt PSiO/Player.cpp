#include "Player.h"
#include "Enemy.h"
#include "Mikstura.h"
#include "Bronie.h"
#include "Pancerze.h"
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>

namespace {

    std::unique_ptr<Item> createItemByName(const std::string& name) {

        if (name == "Slaby Miecz") return std::make_unique<SlabyMiecz>();
        if (name == "Zwykly Miecz") return std::make_unique<ZwyklyMiecz>();
        if (name == "Dobry Miecz") return std::make_unique<DobryMiecz>();
        if (name == "Wysmienity Miecz") return std::make_unique<WysmienityMiecz>();


        if (name == "Kiepski Helm") return std::make_unique<KiepskiHelm>();
        if (name == "Wyborny Helm") return std::make_unique<WybornyHelm>();
        if (name == "Mizerny Napiersnik") return std::make_unique<MizernyNapiersnik>();
        if (name == "Dobry Napiersnik") return std::make_unique<DobryNapiersnik>();
        if (name == "Brudne Spodnie") return std::make_unique<BrudneSpodnie>();
        if (name == "Pancerne Spodnie") return std::make_unique<PancerneSpodnie>();
        if (name == "Trzewiki") return std::make_unique<Trzewiki>();
        if (name == "Wygodne Onuce") return std::make_unique<WygodneOnuce>();


        if (name == "Ma³a mikstura zdrowia") return std::make_unique<MalaMiksturaZdrowia>();
        if (name == "Ma³a mikstura regeneracji zdrowia") return std::make_unique<MalaMiksturaRegeneracjiZdrowia>();
        if (name == "Ma³a mikstura si³y") return std::make_unique<MalaMiksturaSily>();
        if (name == "Ma³a mikstura pancerza") return std::make_unique<MalaMiksturaArmora>();
        if (name == "Duæa mikstura zdrowia") return std::make_unique<DuzaMiksturaZdrowia>();
        if (name == "Duæa mikstura regeneracji zdrowia") return std::make_unique<DuzaMiksturaRegeneracjiZdrowia>();
        if (name == "Duæa mikstura si³y") return std::make_unique<DuzaMiksturaSily>();
        if (name == "Duæa mikstura pancerza") return std::make_unique<DuzaMiksturaArmora>();

        return nullptr;
    }
}

void Player::saveGame(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << "GOLD," << gold << "\n";
    if (inventory.helm) file << "HELM," << inventory.helm->get_nazwa() << "\n";
    if (inventory.klata) file << "KLATA," << inventory.klata->get_nazwa() << "\n";
    if (inventory.spodnie) file << "SPODNIE," << inventory.spodnie->get_nazwa() << "\n";
    if (inventory.buty) file << "BUTY," << inventory.buty->get_nazwa() << "\n";
    if (inventory.bron) file << "BRON," << inventory.bron->get_nazwa() << "\n";

    for (const auto& pot : inventory.mikstury) {
        file << "POTION," << pot->getNazwa() << "\n";
    }

    for (int i = 0; i < 6; ++i) {
        if (inventory.sloty[i]) {
            file << "SLOT_" << i << "," << inventory.sloty[i]->nazwa << "\n";
        }
    }
    file.close();
}

void Player::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;


    inventory.helm.reset(); inventory.klata.reset(); inventory.spodnie.reset();
    inventory.buty.reset(); inventory.bron.reset(); inventory.mikstury.clear();
    for (int i = 0; i < 6; ++i) inventory.sloty[i].reset();

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        size_t delim = line.find(',');
        if (delim == std::string::npos) continue;

        std::string key = line.substr(0, delim);
        std::string val = line.substr(delim + 1);

        if (key == "GOLD") gold = std::stoi(val);
        else {
            auto item = createItemByName(val);
            if (item) {
                if (key == "HELM") inventory.helm.reset(static_cast<Pancerze*>(item.release()));
                else if (key == "KLATA") inventory.klata.reset(static_cast<Pancerze*>(item.release()));
                else if (key == "SPODNIE") inventory.spodnie.reset(static_cast<Pancerze*>(item.release()));
                else if (key == "BUTY") inventory.buty.reset(static_cast<Pancerze*>(item.release()));
                else if (key == "BRON") inventory.bron.reset(static_cast<Bronie*>(item.release()));
                else if (key == "POTION") inventory.mikstury.emplace_back(static_cast<Mikstura*>(item.release()));
                else if (key.find("SLOT_") == 0) {
                    int idx = key[5] - '0';
                    if (idx >= 0 && idx < 6) inventory.sloty[idx] = std::move(item);
                }
            }
        }
    }
    file.close();
}

Player::Player() {
    shape.setRadius(16.f); shape.setOrigin(16.f, 16.f); shape.setFillColor(sf::Color::Transparent);
    pos = sf::Vector2f(400.f, 300.f); speed = 250.f;
    swordShape.setSize(sf::Vector2f(40.f, 6.f)); swordShape.setOrigin(0.f, 3.f); swordShape.setFillColor(sf::Color(200, 200, 200));

    texRun.loadFromFile("textures/Archer_Run.png");
    texShoot.loadFromFile("textures/Archer_Shoot.png");
    swordTex.loadFromFile("textures/Swords.png");
    sprite.setTexture(texRun);

    runFrameWidth = texRun.getSize().x / 4; runFrameHeight = texRun.getSize().y;
    shootFrameWidth = texShoot.getSize().x / 8; shootFrameHeight = texShoot.getSize().y;
    sprite.setOrigin(runFrameWidth / 2.f, runFrameHeight / 2.f);
}

sf::Vector2f Player::getPos() const { return pos; }
float Player::getSpeed() const { return speed; }
float Player::getHp() const { return hp; }
sf::CircleShape Player::getShape() const { return shape; }

void Player::setPos(sf::Vector2f newPos) { pos = newPos; shape.setPosition(pos); }
void Player::setHp(float newHp) { hp = newHp; }

void Player::consumePotion() {
    if (inventory.mikstury.empty()) return;
    Mikstura* m = inventory.mikstury[0].get();
    std::string n = m->getNazwa();

    if (n.find("zdrowia") != std::string::npos && n.find("regeneracji") == std::string::npos) {
        hp += m->getWartosc();
        if (hp > maxHp) hp = maxHp;
    }
    else if (n.find("regeneracji") != std::string::npos) {
        regenTimer = m->getCzas();
        regenRate = m->getWartosc();
    }
    else if (n.find("si") != std::string::npos) {
        strBuffTimer = m->getCzas();
        strBuffAmount = m->getWartosc();
    }
    else if (n.find("pancerza") != std::string::npos) {
        armBuffTimer = m->getCzas();
        armBuffAmount = m->getWartosc();
    }
    inventory.mikstury.erase(inventory.mikstury.begin());
}

void Player::update(float dt, sf::RenderWindow& window, std::vector<Bullet>& bullets, const std::vector<Splash>& splashes, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (regenTimer > 0.f) {
        regenTimer -= dt; hp += regenRate * dt;
        if (hp > maxHp) hp = maxHp;
    }
    if (strBuffTimer > 0.f) strBuffTimer -= dt;
    if (armBuffTimer > 0.f) armBuffTimer -= dt;

    finalDamage = baseDamage;
    finalArmor = baseArmor;

    if (inventory.bron) finalDamage += inventory.bron->get_damage();
    if (inventory.helm) finalArmor += inventory.helm->get_damage();
    if (inventory.klata) finalArmor += inventory.klata->get_damage();
    if (inventory.spodnie) finalArmor += inventory.spodnie->get_damage();
    if (inventory.buty) finalArmor += inventory.buty->get_damage();

    if (strBuffTimer > 0.f) finalDamage += strBuffAmount;
    if (armBuffTimer > 0.f) finalArmor += armBuffAmount;

    isMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { pos.y -= speed * dt; isMoving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { pos.y += speed * dt; isMoving = true; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { pos.x -= speed * dt; isMoving = true; if (!isShooting) facingRight = false; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { pos.x += speed * dt; isMoving = true; if (!isShooting) facingRight = true; }

    shape.setPosition(pos); sprite.setPosition(pos);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && attackCooldown <= 0.f) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f aimDir = sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) - pos;
        float aimLen = std::sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y);
        if (aimLen > 0) { aimDir.x /= aimLen; aimDir.y /= aimLen; }

        bullets.emplace_back(pos, aimDir, 25.f, false, false, false, 1000.f, 4.f);
        attackCooldown = 0.25f;
        isShooting = true; currentFrame = 0; frameTimer = 0.f;
    }

    if (isShooting) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        facingRight = (static_cast<float>(mousePos.x) >= pos.x);

        frameTimer += dt;
        if (frameTimer >= frameDurationShoot) {
            frameTimer = 0.f; currentFrame++;
            if (currentFrame >= 8) { isShooting = false; currentFrame = 0; }
        }

        sprite.setTexture(texShoot);
        sprite.setTextureRect(sf::IntRect(currentFrame * shootFrameWidth, 0, shootFrameWidth, shootFrameHeight));
        sprite.setOrigin(shootFrameWidth / 2.f, shootFrameHeight / 2.f);
    }
    else {
        if (isMoving) {
            frameTimer += dt;
            if (frameTimer >= frameDurationRun) { frameTimer = 0.f; currentFrame = (currentFrame + 1) % 4; }
        }
        else { currentFrame = 0; }

        sprite.setTexture(texRun);
        sprite.setTextureRect(sf::IntRect(currentFrame * runFrameWidth, 0, runFrameWidth, runFrameHeight));
        sprite.setOrigin(runFrameWidth / 2.f, runFrameHeight / 2.f);
    }

    if (facingRight) sprite.setScale(1.f, 1.f); else sprite.setScale(-1.f, 1.f);

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
                    if (!e->getIsInvulnerable()) e->setHp(e->getHp() - finalDamage);
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
            if (s.getIsBomb()) hp -= 200.f * dt;
            else hp -= 15.f * dt;
        }
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);

    sf::RectangleShape hpBg(sf::Vector2f(40.f, 5.f)); hpBg.setFillColor(sf::Color::Red); hpBg.setPosition(pos.x - 20.f, pos.y - shape.getRadius() - 15.f);
    sf::RectangleShape hpFg(sf::Vector2f(40.f * std::max(0.f, hp / maxHp), 5.f)); hpFg.setFillColor(sf::Color::Green); hpFg.setPosition(pos.x - 20.f, pos.y - shape.getRadius() - 15.f);
    window.draw(hpBg); window.draw(hpFg);

    if (swordAnimTimer > 0.f) {
        float progress = 1.0f - (swordAnimTimer / 0.3f);
        float swingAngle = -45.f + (progress * 90.f);
        float finalAngle = swordAttackAngle + swingAngle;

        if (inventory.bron && inventory.bron->get_typ() == "Mele") {
            int colorRow = 4; int length = 0;
            if (inventory.bron->get_nazwa() == "Slaby Miecz") { colorRow = 0; length = 1; }
            else if (inventory.bron->get_nazwa() == "Zwykly Miecz") { colorRow = 2; length = 2; }
            else if (inventory.bron->get_nazwa() == "Dobry Miecz") { colorRow = 1; length = 3; }
            else if (inventory.bron->get_nazwa() == "Wysmienity Miecz") { colorRow = 3; length = 4; }

            int partW = swordTex.getSize().x / 3; int partH = swordTex.getSize().y / 5;
            sf::Sprite s(swordTex); s.setOrigin(0, partH / 2.f);
            float scale = 22.f / static_cast<float>(partW); s.setScale(scale, scale);

            sf::Transform t; t.translate(pos); t.rotate(finalAngle);
            float advance = partW * scale * 0.38f;

            s.setTextureRect(sf::IntRect(0, colorRow * partH, partW, partH));
            window.draw(s, sf::RenderStates(t)); t.translate(advance, 0);

            s.setTextureRect(sf::IntRect(partW, colorRow * partH, partW, partH));
            for (int i = 0; i < length; i++) { window.draw(s, sf::RenderStates(t)); t.translate(advance, 0); }

            s.setTextureRect(sf::IntRect(2 * partW, colorRow * partH, partW, partH));
            window.draw(s, sf::RenderStates(t));
        }
        else {
            swordShape.setPosition(pos); swordShape.setRotation(finalAngle); window.draw(swordShape);
        }
    }
}