#include "Ekwipunek.h"
#include "Player.h"
#include "Pancerze.h"
#include "Bronie.h"
#include "Mikstura.h"

Ekwipunek::Ekwipunek(sf::Font& font, Player* p)
    : font(font), player(p), open(false)
{
    background.setSize({ 500, 450 });
    background.setFillColor(sf::Color(40, 40, 40, 230));
    background.setPosition(200, 100);

    closeButton.setSize({ 40, 40 });
    closeButton.setFillColor(sf::Color(180, 40, 40));
    closeButton.setPosition(640, 120);

    createSlot(helm, "Helm", 0, 0);
    createSlot(chestplate, "Zbroja", 1, 0);
    createSlot(spodnie, "Spodnie", 2, 0);
    createSlot(buty, "Buty", 3, 0);

    createSlot(miecz, "Miecz", 0, 1);
    createSlot(eliksir, "Eliksir", 1, 1);
    createSlot(extra1, "Slot 1", 2, 1);
    createSlot(extra2, "Slot 2", 3, 1);

    createSlot(extra3, "Slot 3", 0, 2);
    createSlot(extra4, "Slot 4", 1, 2);
    createSlot(extra5, "Slot 5", 2, 2);
    createSlot(extra6, "Slot 6", 3, 2);
}

void Ekwipunek::draw(sf::RenderWindow& window) {
    if (!open) return;

    window.draw(background);
    window.draw(closeButton);

    updateSlotLabels();

    drawSlot(window, helm);
    drawSlot(window, chestplate);
    drawSlot(window, spodnie);
    drawSlot(window, buty);

    drawSlot(window, miecz);
    drawSlot(window, eliksir);

    drawSlot(window, extra1);
    drawSlot(window, extra2);
    drawSlot(window, extra3);
    drawSlot(window, extra4);
    drawSlot(window, extra5);
    drawSlot(window, extra6);
}

void Ekwipunek::toggle() {
    open = !open;
}

bool Ekwipunek::isOpen() const {
    return open;
}

void Ekwipunek::handleClick(sf::Vector2i mouse) {
    if (!open) return;

    if (closeButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
        open = false;
    }
}

void Ekwipunek::createSlot(Slot& slot, const std::string& name, int col, int row) {
    float x = 230 + col * 120;
    float y = 180 + row * 120;

    slot.rect.setSize({ 100, 100 });
    slot.rect.setPosition(x, y);
    slot.rect.setFillColor(sf::Color(90, 90, 90));

    slot.label.setFont(font);
    slot.label.setString(name);
    slot.label.setCharacterSize(18);
    slot.label.setFillColor(sf::Color::White);

    sf::FloatRect r = slot.label.getLocalBounds();
    slot.label.setOrigin(r.width / 2, r.height / 2);
    slot.label.setPosition(x + 50, y + 50);
}

void Ekwipunek::drawSlot(sf::RenderWindow& window, Slot& slot) {
    window.draw(slot.rect);
    window.draw(slot.label);
}

void Ekwipunek::updateSlotLabels()
{
    if (player->inventory.helm)
        helm.label.setString(player->inventory.helm->get_nazwa());
    else
        helm.label.setString("Helm");

    if (player->inventory.klata)
        chestplate.label.setString(player->inventory.klata->get_nazwa());
    else
        chestplate.label.setString("Zbroja");

    if (player->inventory.spodnie)
        spodnie.label.setString(player->inventory.spodnie->get_nazwa());
    else
        spodnie.label.setString("Spodnie");

    if (player->inventory.buty)
        buty.label.setString(player->inventory.buty->get_nazwa());
    else
        buty.label.setString("Buty");

    if (player->inventory.bron)
        miecz.label.setString(player->inventory.bron->get_nazwa());
    else
        miecz.label.setString("Miecz");

    if (!player->inventory.mikstury.empty())
        eliksir.label.setString(player->inventory.mikstury[0]->getNazwa());
    else
        eliksir.label.setString("Eliksir");
}
