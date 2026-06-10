#include "Ekwipunek.h"
#include "Player.h"
#include "Item.h"
#include "Pancerze.h"
#include "Bronie.h"
#include "Mikstura.h"
#include <memory>
#include <iostream>

Ekwipunek::Ekwipunek(sf::Font& font, Player* p)
    : font(font), player(p), open(false), draggedIndex(-1)
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

    createSlot(extra1, "Slot", 2, 1);
    createSlot(extra2, "Slot", 3, 1);
    createSlot(extra3, "Slot", 0, 2);
    createSlot(extra4, "Slot", 1, 2);
    createSlot(extra5, "Slot", 2, 2);
    createSlot(extra6, "Slot", 3, 2);
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

    if (closeButton.getGlobalBounds().contains((float)mouse.x, (float)mouse.y)) {
        open = false;
        return;
    }

    Slot* slots[6] = { &extra1, &extra2, &extra3, &extra4, &extra5, &extra6 };

    // --- SLOTY EXTRA: podnoszenie / odk³adanie ---
    for (int i = 0; i < 6; i++) {
        if (slots[i]->rect.getGlobalBounds().contains((float)mouse.x, (float)mouse.y)) {

            // Podnieœ item ze slotu
            if (draggedIndex == -1 && player->inventory.sloty[i]) {
                draggedIndex = i;
                return;
            }

            // Upuœæ item do slotu (pusty lub zajêty)
            if (draggedIndex != -1) {
                player->inventory.sloty[i] = std::move(player->inventory.sloty[draggedIndex]);
                draggedIndex = -1;
                return;
            }
        }
    }

    // --- PODNOSZENIE Z EKWIPUNKU ---
    if (draggedIndex == -1) {
        int freeIdx = player->inventory.findFreeSlot();
        if (freeIdx != -1) {

            if (helm.rect.getGlobalBounds().contains(mouse.x, mouse.y) && player->inventory.helm) {
                player->inventory.sloty[freeIdx] =
                    std::unique_ptr<Item>(player->inventory.helm.release());
                return;
            }

            if (chestplate.rect.getGlobalBounds().contains(mouse.x, mouse.y) && player->inventory.klata) {
                player->inventory.sloty[freeIdx] =
                    std::unique_ptr<Item>(player->inventory.klata.release());
                return;
            }

            if (spodnie.rect.getGlobalBounds().contains(mouse.x, mouse.y) && player->inventory.spodnie) {
                player->inventory.sloty[freeIdx] =
                    std::unique_ptr<Item>(player->inventory.spodnie.release());
                return;
            }

            if (buty.rect.getGlobalBounds().contains(mouse.x, mouse.y) && player->inventory.buty) {
                player->inventory.sloty[freeIdx] =
                    std::unique_ptr<Item>(player->inventory.buty.release());
                return;
            }

            if (miecz.rect.getGlobalBounds().contains(mouse.x, mouse.y) && player->inventory.bron) {
                player->inventory.sloty[freeIdx] =
                    std::unique_ptr<Item>(player->inventory.bron.release());
                return;
            }

            if (eliksir.rect.getGlobalBounds().contains(mouse.x, mouse.y) &&
                !player->inventory.mikstury.empty()) {

                player->inventory.sloty[freeIdx] =
                    std::unique_ptr<Item>(player->inventory.mikstury[0].release());

                player->inventory.mikstury.erase(player->inventory.mikstury.begin());
                return;
            }
        }
    }

    // --- ZAK£ADANIE ITEMÓW ZE SLOTÓW EXTRA ---
    if (draggedIndex != -1) {
        Item* it = player->inventory.sloty[draggedIndex].get();
        if (!it) {
            draggedIndex = -1;
            return;
        }

        // Pancerze
        if (helm.rect.getGlobalBounds().contains(mouse.x, mouse.y) && it->typ == "Helm") {
            auto* p = dynamic_cast<Pancerze*>(player->inventory.sloty[draggedIndex].release());
            if (p) player->inventory.helm.reset(p);
            draggedIndex = -1;
            return;
        }

        if (chestplate.rect.getGlobalBounds().contains(mouse.x, mouse.y) && it->typ == "Klata") {
            auto* p = dynamic_cast<Pancerze*>(player->inventory.sloty[draggedIndex].release());
            if (p) player->inventory.klata.reset(p);
            draggedIndex = -1;
            return;
        }

        if (spodnie.rect.getGlobalBounds().contains(mouse.x, mouse.y) && it->typ == "Spodnie") {
            auto* p = dynamic_cast<Pancerze*>(player->inventory.sloty[draggedIndex].release());
            if (p) player->inventory.spodnie.reset(p);
            draggedIndex = -1;
            return;
        }

        if (buty.rect.getGlobalBounds().contains(mouse.x, mouse.y) && it->typ == "Buty") {
            auto* p = dynamic_cast<Pancerze*>(player->inventory.sloty[draggedIndex].release());
            if (p) player->inventory.buty.reset(p);
            draggedIndex = -1;
            return;
        }

        // Broñ
        if (miecz.rect.getGlobalBounds().contains(mouse.x, mouse.y) &&
            (it->typ == "Mele" || it->typ == "Range")) {

            auto* b = dynamic_cast<Bronie*>(player->inventory.sloty[draggedIndex].release());
            if (b) player->inventory.bron.reset(b);
            draggedIndex = -1;
            return;
        }

        // Mikstura
        if (eliksir.rect.getGlobalBounds().contains(mouse.x, mouse.y) && it->typ == "Mikstura") {
            auto* m = dynamic_cast<Mikstura*>(player->inventory.sloty[draggedIndex].release());
            if (m) player->inventory.mikstury.emplace_back(m);
            draggedIndex = -1;
            return;
        }
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

    //if (player->inventory.helm)
      //  std::cout << "HELM: " << player->inventory.helm->get_nazwa() << " | typ=" << player->inventory.helm->get_typ() << "\n";

    helm.label.setString(player->inventory.helm ? player->inventory.helm->get_nazwa() : "Helm");
    chestplate.label.setString(player->inventory.klata ? player->inventory.klata->get_nazwa() : "Zbroja");
    spodnie.label.setString(player->inventory.spodnie ? player->inventory.spodnie->get_nazwa() : "Spodnie");
    buty.label.setString(player->inventory.buty ? player->inventory.buty->get_nazwa() : "Buty");
    miecz.label.setString(player->inventory.bron ? player->inventory.bron->get_nazwa() : "Miecz");

    if (!player->inventory.mikstury.empty())
        eliksir.label.setString(player->inventory.mikstury[0]->getNazwa());
    else
        eliksir.label.setString("Eliksir");

    Slot* slots[6] = { &extra1, &extra2, &extra3, &extra4, &extra5, &extra6 };

    for (int i = 0; i < 6; i++) {
        if (player->inventory.sloty[i])
            slots[i]->label.setString(player->inventory.sloty[i]->nazwa);
        else
            slots[i]->label.setString("Slot");
    }
}
