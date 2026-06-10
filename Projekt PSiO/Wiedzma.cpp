#include "Wiedzma.h"
#include "Player.h"
#include "Mikstura.h"

Wiedzma::Wiedzma(sf::Font& font, Player* p)
    : font(font), player(p), open(false)
{
    background.setSize({ 500, 400 });
    background.setFillColor(sf::Color(40, 40, 40, 230));
    background.setPosition(200, 50);

    closeButton.setSize({ 40, 40 });
    closeButton.setFillColor(sf::Color(180, 40, 40));
    closeButton.setPosition(640, 70);

    createSlot(pot_hp_small, "Mala HP", 0, 0);
    createSlot(pot_hp_big, "Duza HP", 1, 0);

    createSlot(pot_rhp_small, "Mala R.HP", 0, 1);
    createSlot(pot_rhp_big, "Duza R.HP", 1, 1);

    createSlot(pot_str_small, "Mala STR", 0, 2);
    createSlot(pot_str_big, "Duza STR", 1, 2);

    createSlot(pot_arm_small, "Mala ARM", 0, 3);
    createSlot(pot_arm_big, "Duza ARM", 1, 3);
}

void Wiedzma::draw(sf::RenderWindow& window) {
    if (!open) return;

    window.draw(background);
    window.draw(closeButton);

    drawSlot(window, pot_hp_small);
    drawSlot(window, pot_hp_big);

    drawSlot(window, pot_rhp_small);
    drawSlot(window, pot_rhp_big);

    drawSlot(window, pot_str_small);
    drawSlot(window, pot_str_big);

    drawSlot(window, pot_arm_small);
    drawSlot(window, pot_arm_big);
}

void Wiedzma::toggle() {
    open = !open;
}

bool Wiedzma::isOpen() const {
    return open;
}

void Wiedzma::handleClick(sf::Vector2i mouse) {
    if (!open) return;

    if (closeButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
        open = false;
        return;
    }

    if (pot_hp_small.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.addToFreeSlot(std::make_unique<MalaMiksturaZdrowia>());

    if (pot_hp_big.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.addToFreeSlot(std::make_unique<DuzaMiksturaZdrowia>());

    if (pot_rhp_small.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.addToFreeSlot(std::make_unique<MalaMiksturaRegeneracjiZdrowia>());

    if (pot_rhp_big.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.addToFreeSlot(std::make_unique<DuzaMiksturaRegeneracjiZdrowia>());

    if (pot_str_small.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.addToFreeSlot(std::make_unique<MalaMiksturaSily>());

    if (pot_str_big.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.addToFreeSlot(std::make_unique<DuzaMiksturaSily>());

    if (pot_arm_small.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.addToFreeSlot(std::make_unique<MalaMiksturaArmora>());

    if (pot_arm_big.rect.getGlobalBounds().contains(mouse.x, mouse.y))
        player->inventory.addToFreeSlot(std::make_unique<DuzaMiksturaArmora>());
}

void Wiedzma::createSlot(Slot& slot, const std::string& name, int col, int row) {
    float x = 230 + col * 150;
    float y = 120 + row * 80;

    slot.rect.setSize({ 130, 60 });
    slot.rect.setPosition(x, y);
    slot.rect.setFillColor(sf::Color(90, 90, 90));

    slot.label.setFont(font);
    slot.label.setString(name);
    slot.label.setCharacterSize(18);
    slot.label.setFillColor(sf::Color::White);

    sf::FloatRect r = slot.label.getLocalBounds();
    slot.label.setOrigin(r.width / 2, r.height / 2);
    slot.label.setPosition(x + 65, y + 30);
}

void Wiedzma::drawSlot(sf::RenderWindow& window, Slot& slot) {
    window.draw(slot.rect);
    window.draw(slot.label);
}
