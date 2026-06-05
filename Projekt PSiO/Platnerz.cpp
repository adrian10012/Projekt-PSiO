#include "Platnerz.h"
#include "Player.h"
#include "Pancerze.h"

Platnerz::Platnerz(sf::Font& font, Player* p)
    : font(font), player(p), open(false)
{
    background.setSize({ 500, 450 });
    background.setFillColor(sf::Color(40, 40, 40, 230));
    background.setPosition(200, 100);

    closeButton.setSize({ 40, 40 });
    closeButton.setFillColor(sf::Color(180, 40, 40));
    closeButton.setPosition(640, 120);

    createSlot(helm1, "Helm 1", 0, 0);
    createSlot(helm2, "Helm 2", 1, 0);

    createSlot(chest1, "Zbroja 1", 0, 1);
    createSlot(chest2, "Zbroja 2", 1, 1);

    createSlot(buty1, "Buty 1", 0, 2);
    createSlot(buty2, "Buty 2", 1, 2);

    createSlot(spodnie1, "Spodnie 1", 0, 3);
    createSlot(spodnie2, "Spodnie 2", 1, 3);
}

void Platnerz::draw(sf::RenderWindow& window) {
    if (!open) return;

    window.draw(background);
    window.draw(closeButton);

    drawSlot(window, helm1);
    drawSlot(window, helm2);

    drawSlot(window, chest1);
    drawSlot(window, chest2);

    drawSlot(window, buty1);
    drawSlot(window, buty2);

    drawSlot(window, spodnie1);
    drawSlot(window, spodnie2);
}

void Platnerz::toggle() {
    open = !open;
}

bool Platnerz::isOpen() const {
    return open;
}

void Platnerz::handleClick(sf::Vector2i mouse) {
    if (!open) return;

    if (closeButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
        open = false;
        return;
    }

    if (helm1.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.equipArmor(std::make_unique<KiepskiHelm>());

    if (helm2.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.equipArmor(std::make_unique<WybornyHelm>());

    if (chest1.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.equipArmor(std::make_unique<MizernyNapiersnik>());

    if (chest2.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.equipArmor(std::make_unique<DobryNapiersnik>());

    if (buty1.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.equipArmor(std::make_unique<Trzewiki>());

    if (buty2.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.equipArmor(std::make_unique<WygodneOnuce>());

    if (spodnie1.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.equipArmor(std::make_unique<BrudneSpodnie>());

    if (spodnie2.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.equipArmor(std::make_unique<PancerneSpodnie>());
}

void Platnerz::createSlot(Slot& slot, const std::string& name, int col, int row) {
    float x = 230 + col * 150;
    float y = 150 + row * 90;

    slot.rect.setSize({ 130, 70 });
    slot.rect.setPosition(x, y);
    slot.rect.setFillColor(sf::Color(90, 90, 90));

    slot.label.setFont(font);
    slot.label.setString(name);
    slot.label.setCharacterSize(18);
    slot.label.setFillColor(sf::Color::White);

    sf::FloatRect r = slot.label.getLocalBounds();
    slot.label.setOrigin(r.width / 2, r.height / 2);
    slot.label.setPosition(x + 65, y + 35);
}

void Platnerz::drawSlot(sf::RenderWindow& window, Slot& slot) {
    window.draw(slot.rect);
    window.draw(slot.label);
}
