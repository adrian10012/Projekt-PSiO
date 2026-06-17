#include "Kowal.h"
#include "Player.h"
#include "Bronie.h"
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
                    int mX = eX, mY = eY, M_X = sX, M_Y = sY; bool f = false;
                    for (int y = sY; y < eY; ++y) for (int x = sX; x < eX; ++x) {
                        if (img.getPixel(x, y).a > 10) {
                            if (x < mX) mX = x; if (x > M_X) M_X = x;
                            if (y < mY) mY = y; if (y > M_Y) M_Y = y; f = true;
                        }
                    }
                    if (f) r[i * 3 + j] = sf::IntRect(mX, mY, M_X - mX + 1, M_Y - mY + 1);
                    else r[i * 3 + j] = sf::IntRect(sX, sY, 1, 1);
                }
            }
            sW[0] = r[0].width; sW[1] = r[1].width; sW[2] = r[2].width;
            sH[0] = r[0].height; sH[1] = r[3].height; sH[2] = r[6].height; valid = true;
        }
        void draw(sf::RenderWindow& w, const sf::Texture& t, float x, float y, float wd, float ht) {
            if (!valid) return; sf::Sprite s(t);
            float scX = 1.f, scY = 1.f;
            if (wd < sW[0] + sW[2]) scX = wd / (sW[0] + sW[2]); if (ht < sH[0] + sH[2]) scY = ht / (sH[0] + sH[2]);
            float dW[3] = { sW[0] * scX, std::max(0.f, wd - (sW[0] + sW[2]) * scX), sW[2] * scX };
            float dH[3] = { sH[0] * scY, std::max(0.f, ht - (sH[0] + sH[2]) * scY), sH[2] * scY };
            float dX[3] = { x, x + dW[0], x + dW[0] + dW[1] }; float dY[3] = { y, y + dH[0], y + dH[0] + dH[1] };
            for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) {
                if (dW[j] <= 0 || dH[i] <= 0) continue; s.setTextureRect(r[i * 3 + j]); s.setPosition(dX[j], dY[i]);
                s.setScale((dW[j] + (j == 1 ? 0.5f : 0.f)) / sW[j], (dH[i] + (i == 1 ? 0.5f : 0.f)) / sH[i]); w.draw(s);
            }
        }
    };
    std::map<const sf::Texture*, CachedNineSlice> nsCache;
    void drawSmartUI(sf::RenderWindow& win, const sf::Texture& tex, float x, float y, float w, float h) {
        if (!nsCache[&tex].built) nsCache[&tex].build(tex); nsCache[&tex].draw(win, tex, x, y, w, h);
    }
}

Kowal::Kowal(sf::Font& font, Player* p)
    : font(font), player(p), open(false)
{
    bgTex.loadFromFile("textures/BigBlueButton_Regular.png");
    closeTex.loadFromFile("textures/BigRedButton_Regular.png");
    slotTex.loadFromFile("textures/BigBlueButton_Pressed.png");

    bgBounds = sf::FloatRect(200.f, 150.f, 500.f, 350.f);
    closeBounds = sf::FloatRect(640.f, 170.f, 40.f, 40.f);

    createSlot(sword1, "Slaby Miecz", 0, 0, 50, "textures/sword_01a.png");
    createSlot(sword2, "Zwykly Miecz", 1, 0, 150, "textures/sword_01b.png");
    createSlot(sword3, "Dobry Miecz", 0, 1, 300, "textures/sword_01c.png");
    createSlot(sword4, "Wysmienity Miecz", 1, 1, 600, "textures/sword_01e.png");
}

void Kowal::draw(sf::RenderWindow& window) {
    if (!open) return;
    drawSmartUI(window, bgTex, bgBounds.left, bgBounds.top, bgBounds.width, bgBounds.height);
    drawSmartUI(window, closeTex, closeBounds.left, closeBounds.top, closeBounds.width, closeBounds.height);
    drawSlot(window, sword1); drawSlot(window, sword2); drawSlot(window, sword3); drawSlot(window, sword4);

    sf::Text goldTxt;
    goldTxt.setFont(font); goldTxt.setString("Zloto: " + std::to_string(player->gold));
    goldTxt.setCharacterSize(22); goldTxt.setFillColor(sf::Color::Yellow);
    goldTxt.setPosition(bgBounds.left + 20.f, bgBounds.top + 15.f);
    window.draw(goldTxt);
}

void Kowal::toggle() { open = !open; }
bool Kowal::isOpen() const { return open; }

void Kowal::handleClick(sf::Vector2i mouse) {
    if (!open) return;
    float mx = static_cast<float>(mouse.x);
    float my = static_cast<float>(mouse.y);

    if (closeBounds.contains(mx, my)) { open = false; return; }

    auto tryBuy = [&](Slot& s, auto createItem) {
        if (s.bounds.contains(mx, my)) {
            if (player->gold >= s.price && player->inventory.findFreeSlot() != -1) {
                player->gold -= s.price;
                player->inventory.addToFreeSlot(createItem());
            }
        }
        };

    tryBuy(sword1, []() { return std::make_unique<SlabyMiecz>(); });
    tryBuy(sword2, []() { return std::make_unique<ZwyklyMiecz>(); });
    tryBuy(sword3, []() { return std::make_unique<DobryMiecz>(); });
    tryBuy(sword4, []() { return std::make_unique<WysmienityMiecz>(); });
}

void Kowal::createSlot(Slot& slot, const std::string& name, int col, int row, int price, const std::string& texPath) {
    float x = 230 + col * 150; float y = 220 + row * 100;
    slot.bounds = sf::FloatRect(x, y, 130.f, 80.f);

    slot.label.setFont(font); slot.label.setString(name);
    slot.label.setCharacterSize(14); slot.label.setFillColor(sf::Color::White);
    sf::FloatRect r = slot.label.getLocalBounds(); slot.label.setOrigin(r.width / 2.f, r.height / 2.f);
    slot.label.setPosition(x + 65.f, y + 60.f);

    slot.price = price;
    slot.priceLabel.setFont(font); slot.priceLabel.setString(std::to_string(price) + "g");
    slot.priceLabel.setCharacterSize(16); slot.priceLabel.setFillColor(sf::Color::Yellow);
    sf::FloatRect pr = slot.priceLabel.getLocalBounds(); slot.priceLabel.setOrigin(pr.width / 2.f, pr.height / 2.f);
    slot.priceLabel.setPosition(x + 65.f, y + 10.f);

    slot.itemIcon.setColor(sf::Color::Transparent);
    if (!texPath.empty()) {
        static std::map<std::string, sf::Texture> texMap;
        if (texMap.find(texPath) == texMap.end()) texMap[texPath].loadFromFile(texPath);
        slot.itemIcon.setTexture(texMap[texPath]);
        slot.itemIcon.setOrigin(texMap[texPath].getSize().x / 2.f, texMap[texPath].getSize().y / 2.f);
        slot.itemIcon.setPosition(x + 65.f, y + 35.f); slot.itemIcon.setScale(1.5f, 1.5f);
        slot.itemIcon.setColor(sf::Color::White);
    }
}

void Kowal::drawSlot(sf::RenderWindow& window, Slot& slot) {
    drawSmartUI(window, slotTex, slot.bounds.left, slot.bounds.top, slot.bounds.width, slot.bounds.height);
    if (slot.itemIcon.getColor() != sf::Color::Transparent) window.draw(slot.itemIcon);
    window.draw(slot.label); window.draw(slot.priceLabel);
}