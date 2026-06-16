#include "Platnerz.h"
#include "Player.h"
#include "Pancerze.h"
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
}

Platnerz::Platnerz(sf::Font& font, Player* p)
    : font(font), player(p), open(false)
{
    bgTex.loadFromFile("textures/BigBlueButton_Regular.png");
    closeTex.loadFromFile("textures/BigRedButton_Regular.png");
    slotTex.loadFromFile("textures/BigBlueButton_Pressed.png");

    bgBounds = sf::FloatRect(200.f, 100.f, 500.f, 450.f);
    closeBounds = sf::FloatRect(640.f, 120.f, 40.f, 40.f);

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

    drawSmartUI(window, bgTex, bgBounds.left, bgBounds.top, bgBounds.width, bgBounds.height);
    drawSmartUI(window, closeTex, closeBounds.left, closeBounds.top, closeBounds.width, closeBounds.height);

    drawSlot(window, helm1);
    drawSlot(window, helm2);
    drawSlot(window, chest1);
    drawSlot(window, chest2);
    drawSlot(window, buty1);
    drawSlot(window, buty2);
    drawSlot(window, spodnie1);
    drawSlot(window, spodnie2);
}

void Platnerz::toggle() { open = !open; }
bool Platnerz::isOpen() const { return open; }

void Platnerz::handleClick(sf::Vector2i mouse) {
    if (!open) return;
    float mx = static_cast<float>(mouse.x);
    float my = static_cast<float>(mouse.y);

    if (closeBounds.contains(mx, my)) {
        open = false; return;
    }

    if (helm1.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<KiepskiHelm>());
    if (helm2.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<WybornyHelm>());
    if (chest1.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<MizernyNapiersnik>());
    if (chest2.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<DobryNapiersnik>());
    if (buty1.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<Trzewiki>());
    if (buty2.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<WygodneOnuce>());
    if (spodnie1.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<BrudneSpodnie>());
    if (spodnie2.bounds.contains(mx, my)) player->inventory.addToFreeSlot(std::make_unique<PancerneSpodnie>());
}

void Platnerz::createSlot(Slot& slot, const std::string& name, int col, int row) {
    float x = 230 + col * 150;
    float y = 150 + row * 90;

    slot.bounds = sf::FloatRect(x, y, 130.f, 70.f);

    slot.label.setFont(font);
    slot.label.setString(name);
    slot.label.setCharacterSize(18);
    slot.label.setFillColor(sf::Color::White);

    sf::FloatRect r = slot.label.getLocalBounds();
    slot.label.setOrigin(r.width / 2, r.height / 2);
    slot.label.setPosition(x + 65, y + 35);
}

void Platnerz::drawSlot(sf::RenderWindow& window, Slot& slot) {
    drawSmartUI(window, slotTex, slot.bounds.left, slot.bounds.top, slot.bounds.width, slot.bounds.height);
    window.draw(slot.label);
}