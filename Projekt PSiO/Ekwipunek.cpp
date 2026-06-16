#include "Ekwipunek.h"
#include "Player.h"
#include "Item.h"
#include "Pancerze.h"
#include "Bronie.h"
#include "Mikstura.h"
#include <memory>
#include <map>
#include <vector>

namespace {
    struct CachedNineSlice {
        sf::IntRect r[9]; float sW[3], sH[3]; bool valid = false, built = false;
        void build(const sf::Texture& tex) {
            built = true; sf::Image img = tex.copyToImage();
            int cW = img.getSize().x / 3, cH = img.getSize().y / 3;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    int sX = j * cW, sY = i * cH, eX = sX + cW, eY = sY + cH;
                    int mX = eX, mY = eY, M_X = sX, M_Y = sY;
                    bool f = false;
                    for (int y = sY; y < eY; ++y) {
                        for (int x = sX; x < eX; ++x) {
                            if (img.getPixel(x, y).a > 10) {
                                if (x < mX) mX = x; if (x > M_X) M_X = x;
                                if (y < mY) mY = y; if (y > M_Y) M_Y = y;
                                f = true;
                            }
                        }
                    }
                    if (f) r[i * 3 + j] = sf::IntRect(mX, mY, M_X - mX + 1, M_Y - mY + 1);
                    else r[i * 3 + j] = sf::IntRect(sX, sY, 1, 1);
                }
            }
            sW[0] = r[0].width; sW[1] = r[1].width; sW[2] = r[2].width;
            sH[0] = r[0].height; sH[1] = r[3].height; sH[2] = r[6].height;
            valid = true;
        }
        void draw(sf::RenderWindow& w, const sf::Texture& t, float x, float y, float wd, float ht) {
            if (!valid) return;
            sf::Sprite s(t);
            float scX = 1.f, scY = 1.f;
            if (wd < sW[0] + sW[2]) scX = wd / (sW[0] + sW[2]);
            if (ht < sH[0] + sH[2]) scY = ht / (sH[0] + sH[2]);
            float dW[3] = { sW[0] * scX, std::max(0.f, wd - (sW[0] + sW[2]) * scX), sW[2] * scX };
            float dH[3] = { sH[0] * scY, std::max(0.f, ht - (sH[0] + sH[2]) * scY), sH[2] * scY };
            float dX[3] = { x, x + dW[0], x + dW[0] + dW[1] };
            float dY[3] = { y, y + dH[0], y + dH[0] + dH[1] };
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (dW[j] <= 0 || dH[i] <= 0) continue;
                    s.setTextureRect(r[i * 3 + j]);
                    s.setPosition(dX[j], dY[i]);
                    s.setScale((dW[j] + (j == 1 ? 0.5f : 0.f)) / sW[j], (dH[i] + (i == 1 ? 0.5f : 0.f)) / sH[i]);
                    w.draw(s);
                }
            }
        }
    };
    std::map<const sf::Texture*, CachedNineSlice> nsCache;
    void drawSmartUI(sf::RenderWindow& win, const sf::Texture& tex, float x, float y, float w, float h) {
        if (!nsCache[&tex].built) nsCache[&tex].build(tex);
        nsCache[&tex].draw(win, tex, x, y, w, h);
    }

    sf::IntRect getPotionRect(const sf::Texture& tex, int col, int row) {
        // --- TUTAJ ZMIENIASZ PARAMETRY WYTNIÊCIA ---
        int cellW = 16; // Szerokoœæ jednej "kratki" z potk¹ (zmieñ np. na 24 lub 32 jeœli tnie Ÿle)
        int cellH = 16; // Wysokoœæ jednej "kratki" z potk¹

        int offsetX = 0; // Jeœli ca³a siatka jest przesuniêta o np. 1 piksel w prawo, ustaw na 1
        int offsetY = 0; // Jeœli przesuniêta w dó³, ustaw na 1

        int sX = offsetX + (col * cellW);
        int sY = offsetY + (row * cellH);

        return sf::IntRect(sX, sY, cellW, cellH);
    }
}

Ekwipunek::Ekwipunek(sf::Font& font, Player* p)
    : font(font), player(p), open(false), draggedIndex(-1)
{
    bgTex.loadFromFile("textures/WoodTable.png");
    closeTex.loadFromFile("textures/BigRedButton_Regular.png");
    slotTex.loadFromFile("textures/BigBlueButton_Pressed.png");
    potionsTex.loadFromFile("textures/potions.png");

    bgBounds = sf::FloatRect(200.f, 100.f, 500.f, 450.f);
    closeBounds = sf::FloatRect(640.f, 120.f, 40.f, 40.f);

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

    drawSmartUI(window, bgTex, bgBounds.left, bgBounds.top, bgBounds.width, bgBounds.height);
    drawSmartUI(window, closeTex, closeBounds.left, closeBounds.top, closeBounds.width, closeBounds.height);

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

void Ekwipunek::toggle() { open = !open; }
bool Ekwipunek::isOpen() const { return open; }

void Ekwipunek::handleClick(sf::Vector2i mouse) {
    if (!open) return;
    float mx = static_cast<float>(mouse.x);
    float my = static_cast<float>(mouse.y);

    if (closeBounds.contains(mx, my)) {
        open = false; return;
    }

    Slot* slots[6] = { &extra1, &extra2, &extra3, &extra4, &extra5, &extra6 };

    for (int i = 0; i < 6; i++) {
        if (slots[i]->bounds.contains(mx, my)) {
            if (draggedIndex == -1 && player->inventory.sloty[i]) {
                draggedIndex = i; return;
            }
            if (draggedIndex != -1) {
                player->inventory.sloty[i] = std::move(player->inventory.sloty[draggedIndex]);
                draggedIndex = -1; return;
            }
        }
    }

    if (draggedIndex == -1) {
        int freeIdx = player->inventory.findFreeSlot();
        if (freeIdx != -1) {
            if (helm.bounds.contains(mx, my) && player->inventory.helm) {
                player->inventory.sloty[freeIdx] = std::unique_ptr<Item>(player->inventory.helm.release()); return;
            }
            if (chestplate.bounds.contains(mx, my) && player->inventory.klata) {
                player->inventory.sloty[freeIdx] = std::unique_ptr<Item>(player->inventory.klata.release()); return;
            }
            if (spodnie.bounds.contains(mx, my) && player->inventory.spodnie) {
                player->inventory.sloty[freeIdx] = std::unique_ptr<Item>(player->inventory.spodnie.release()); return;
            }
            if (buty.bounds.contains(mx, my) && player->inventory.buty) {
                player->inventory.sloty[freeIdx] = std::unique_ptr<Item>(player->inventory.buty.release()); return;
            }
            if (miecz.bounds.contains(mx, my) && player->inventory.bron) {
                player->inventory.sloty[freeIdx] = std::unique_ptr<Item>(player->inventory.bron.release()); return;
            }
            if (eliksir.bounds.contains(mx, my) && !player->inventory.mikstury.empty()) {
                player->inventory.sloty[freeIdx] = std::unique_ptr<Item>(player->inventory.mikstury[0].release());
                player->inventory.mikstury.erase(player->inventory.mikstury.begin());
                return;
            }
        }
    }

    if (draggedIndex != -1) {
        Item* it = player->inventory.sloty[draggedIndex].get();
        if (!it) { draggedIndex = -1; return; }

        if (helm.bounds.contains(mx, my) && it->typ == "Helm") {
            auto* p = dynamic_cast<Pancerze*>(player->inventory.sloty[draggedIndex].release());
            if (p) player->inventory.helm.reset(p);
            draggedIndex = -1; return;
        }
        if (chestplate.bounds.contains(mx, my) && it->typ == "Klata") {
            auto* p = dynamic_cast<Pancerze*>(player->inventory.sloty[draggedIndex].release());
            if (p) player->inventory.klata.reset(p);
            draggedIndex = -1; return;
        }
        if (spodnie.bounds.contains(mx, my) && it->typ == "Spodnie") {
            auto* p = dynamic_cast<Pancerze*>(player->inventory.sloty[draggedIndex].release());
            if (p) player->inventory.spodnie.reset(p);
            draggedIndex = -1; return;
        }
        if (buty.bounds.contains(mx, my) && it->typ == "Buty") {
            auto* p = dynamic_cast<Pancerze*>(player->inventory.sloty[draggedIndex].release());
            if (p) player->inventory.buty.reset(p);
            draggedIndex = -1; return;
        }
        if (miecz.bounds.contains(mx, my) && (it->typ == "Mele" || it->typ == "Range")) {
            auto* b = dynamic_cast<Bronie*>(player->inventory.sloty[draggedIndex].release());
            if (b) player->inventory.bron.reset(b);
            draggedIndex = -1; return;
        }
        if (eliksir.bounds.contains(mx, my) && it->typ == "Mikstura") {
            auto* m = dynamic_cast<Mikstura*>(player->inventory.sloty[draggedIndex].release());
            if (m) player->inventory.mikstury.emplace_back(m);
            draggedIndex = -1; return;
        }
    }
}

void Ekwipunek::createSlot(Slot& slot, const std::string& name, int col, int row) {
    float x = 230 + col * 120;
    float y = 180 + row * 120;

    slot.bounds = sf::FloatRect(x, y, 100.f, 100.f);

    slot.label.setFont(font);
    slot.label.setString(name);
    slot.label.setCharacterSize(16);
    slot.label.setFillColor(sf::Color::White);

    slot.itemIcon.setColor(sf::Color::Transparent);
}

void Ekwipunek::drawSlot(sf::RenderWindow& window, Slot& slot) {
    drawSmartUI(window, slotTex, slot.bounds.left, slot.bounds.top, slot.bounds.width, slot.bounds.height);
    if (slot.itemIcon.getColor() != sf::Color::Transparent) window.draw(slot.itemIcon);
    else window.draw(slot.label);
}

void Ekwipunek::updateSlotLabels()
{
    auto setupSlot = [&](Slot& s, Item* item, const std::string& defaultName) {
        if (!item) {
            s.label.setString(defaultName);
            s.itemIcon.setColor(sf::Color::Transparent);
        }
        else if (item->typ == "Mikstura") {
            s.itemIcon.setTexture(potionsTex);
            int r = 2, c = 3; std::string n = item->nazwa;

            if (n.find("zdrowia") != std::string::npos && n.find("regeneracji") == std::string::npos) c = 3;
            else if (n.find("regeneracji") != std::string::npos) c = 14;
            else if (n.find("si") != std::string::npos) c = 4;
            else if (n.find("pancerza") != std::string::npos) c = 11;

            if (n.find("Ma") != std::string::npos) r = 10;
            else r = 2;

            sf::IntRect pRect = getPotionRect(potionsTex, c, r);
            s.itemIcon.setTextureRect(pRect);
            s.itemIcon.setOrigin(pRect.width / 2.f, pRect.height / 2.f);
            s.itemIcon.setPosition(s.bounds.left + s.bounds.width / 2.f, s.bounds.top + s.bounds.height / 2.f);
            s.itemIcon.setScale(2.5f, 2.5f);
            s.itemIcon.setColor(sf::Color::White);
        }
        else {
            s.label.setString(item->nazwa);
            s.itemIcon.setColor(sf::Color::Transparent);
        }

        sf::FloatRect rBound = s.label.getLocalBounds();
        s.label.setOrigin(rBound.width / 2.f, rBound.height / 2.f);
        s.label.setPosition(s.bounds.left + s.bounds.width / 2.f, s.bounds.top + s.bounds.height / 2.f);
        };

    setupSlot(helm, player->inventory.helm.get(), "Helm");
    setupSlot(chestplate, player->inventory.klata.get(), "Zbroja");
    setupSlot(spodnie, player->inventory.spodnie.get(), "Spodnie");
    setupSlot(buty, player->inventory.buty.get(), "Buty");
    setupSlot(miecz, player->inventory.bron.get(), "Miecz");

    Item* pot = player->inventory.mikstury.empty() ? nullptr : player->inventory.mikstury[0].get();
    setupSlot(eliksir, pot, "Eliksir");

    Slot* slots[6] = { &extra1, &extra2, &extra3, &extra4, &extra5, &extra6 };
    for (int i = 0; i < 6; i++) {
        setupSlot(*slots[i], player->inventory.sloty[i].get(), "Slot");
    }
}