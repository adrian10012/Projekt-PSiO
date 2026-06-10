#include "Kowal.h"
#include "Player.h"
#include "Bronie.h"

Kowal::Kowal(sf::Font& font, Player* p)
    : font(font), player(p), open(false)
{
    background.setSize({ 500, 350 });
    background.setFillColor(sf::Color(40, 40, 40, 230));
    background.setPosition(200, 150);

    closeButton.setSize({ 40, 40 });
    closeButton.setFillColor(sf::Color(180, 40, 40));
    closeButton.setPosition(640, 170);

    createSlot(sword1, "Slaby Miecz", 0, 0);
    createSlot(sword2, "Zwykly Miecz", 1, 0);
    createSlot(sword3, "Dobry Miecz", 0, 1);
    createSlot(sword4, "Wysmienity Miecz", 1, 1);
}

void Kowal::draw(sf::RenderWindow& window) {
    if (!open) return;

    window.draw(background);
    window.draw(closeButton);

    drawSlot(window, sword1);
    drawSlot(window, sword2);
    drawSlot(window, sword3);
    drawSlot(window, sword4);
}

void Kowal::toggle() {
    open = !open;
}

bool Kowal::isOpen() const {
    return open;
}

void Kowal::handleClick(sf::Vector2i mouse) {
    if (!open) return;

    if (closeButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
        open = false;
        return;
    }

    if (sword1.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.addToFreeSlot(std::make_unique<SlabyMiecz>());

    if (sword2.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.addToFreeSlot(std::make_unique<ZwyklyMiecz>());

    if (sword3.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.addToFreeSlot(std::make_unique<DobryMiecz>());

    if (sword4.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.addToFreeSlot(std::make_unique<WysmienityMiecz>());
}

void Kowal::createSlot(Slot& slot, const std::string& name, int col, int row) {
    float x = 230 + col * 150;
    float y = 220 + row * 100;

    slot.rect.setSize({ 130, 80 });
    slot.rect.setPosition(x, y);
    slot.rect.setFillColor(sf::Color(90, 90, 90));

    slot.label.setFont(font);
    slot.label.setString(name);
    slot.label.setCharacterSize(20);
    slot.label.setFillColor(sf::Color::White);

    sf::FloatRect r = slot.label.getLocalBounds();
    slot.label.setOrigin(r.width / 2, r.height / 2);
    slot.label.setPosition(x + 65, y + 40);
}

void Kowal::drawSlot(sf::RenderWindow& window, Slot& slot) {
    window.draw(slot.rect);
    window.draw(slot.label);
}
