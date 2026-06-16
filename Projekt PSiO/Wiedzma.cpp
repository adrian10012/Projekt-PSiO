#include "Wiedzma.h"
#include "Player.h"
#include "Mikstura.h"
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

    // --- POPRAWIONE WYCINANIE POTEK NA SZTYWNO ---
    sf::IntRect getPotionRect(const sf::Texture& tex, int col, int row) {
        int cellW = 16; // Wymiar ikonki z pliku
        int cellH = 16; // Wymiar ikonki z pliku
        return sf::IntRect(col * cellW, row * cellH, cellW, cellH);
    }
}

Wiedzma::Wiedzma(sf::Font& font, Player* p)
    : font(font), player(p), open(false)
{
    bgTex.loadFromFile("textures/BigBlueButton_Regular.png");
    closeTex.loadFromFile("textures/BigRedButton_Regular.png");
    slotTex.loadFromFile("textures/BigBlueButton_Pressed.png");
    potionsTex.loadFromFile("textures/potions.png");

    bgBounds = sf::FloatRect(200.f, 50.f, 500.f, 400.f);
    closeBounds = sf::FloatRect(640.f, 70.f, 40.f, 40.f);

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
    drawSmartUI(window, bgTex, bgBounds.left, bgBounds.top, bgBounds.width, bgBounds.height);
    drawSmartUI(window, closeTex, closeBounds.left, closeBounds.top, closeBounds.width, closeBounds.height);
    drawSlot(window, pot_hp_small);
    drawSlot(window, pot_hp_big);
    drawSlot(window, pot_rhp_small);
    drawSlot(window, pot_rhp_big);
    drawSlot(window, pot_str_small);
    drawSlot(window, pot_str_big);
    drawSlot(window, pot_arm_small);
    drawSlot(window, pot_arm_big);
}

void Wiedzma::toggle() { open = !open; }
bool Wiedzma::isOpen() const { return open; }

void Wiedzma::handleClick(sf::Vector2i mouse) {
    if (!open) return;
    float mx = static_cast<float>(mouse.x);
    float my = static_cast<float>(mouse.y);

    if (closeBounds.contains(mx, my)) {
        open = false; return;
    }

    if (pot_hp_small.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<MalaMiksturaZdrowia>());
    if (pot_hp_big.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<DuzaMiksturaZdrowia>());
    if (pot_rhp_small.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<MalaMiksturaRegeneracjiZdrowia>());
    if (pot_rhp_big.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<DuzaMiksturaRegeneracjiZdrowia>());
    if (pot_str_small.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<MalaMiksturaSily>());
    if (pot_str_big.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<DuzaMiksturaSily>());
    if (pot_arm_small.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<MalaMiksturaArmora>());
    if (pot_arm_big.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<DuzaMiksturaArmora>());
}

void Wiedzma::createSlot(Slot& slot, const std::string& name, int col, int row) {
    float x = 230 + col * 150;
    float y = 120 + row * 80;

    slot.bounds = sf::FloatRect(x, y, 130.f, 65.f);

    int texC = 0, texR = 0;
    if (name == "Mala HP") { texC = 3; texR = 10; }
    else if (name == "Duza HP") { texC = 3; texR = 2; }
    else if (name == "Mala R.HP") { texC = 14; texR = 10; }
    else if (name == "Duza R.HP") { texC = 14; texR = 2; }
    else if (name == "Mala STR") { texC = 4; texR = 10; }
    else if (name == "Duza STR") { texC = 4; texR = 2; }
    else if (name == "Mala ARM") { texC = 11; texR = 10; }
    else if (name == "Duza ARM") { texC = 11; texR = 2; }

    sf::IntRect pRect = getPotionRect(potionsTex, texC, texR);
    slot.itemIcon.setTexture(potionsTex);
    slot.itemIcon.setTextureRect(pRect);
    slot.itemIcon.setOrigin(pRect.width / 2.f, pRect.height / 2.f);
    slot.itemIcon.setPosition(x + 65.f, y + 25.f); // Ikona podniesiona
    slot.itemIcon.setScale(2.5f, 2.5f); // £adne powiêkszenie

    slot.label.setFont(font);
    slot.label.setString(name);
    slot.label.setCharacterSize(13);
    slot.label.setFillColor(sf::Color::White);

    sf::FloatRect r = slot.label.getLocalBounds();
    slot.label.setOrigin(r.width / 2.f, r.height / 2.f);
    slot.label.setPosition(x + 65.f, y + 53.f); // Tekst elegancko na dole slota
}

void Wiedzma::drawSlot(sf::RenderWindow& window, Slot& slot) {
    drawSmartUI(window, slotTex, slot.bounds.left, slot.bounds.top, slot.bounds.width, slot.bounds.height);
    window.draw(slot.itemIcon);
    window.draw(slot.label);
}