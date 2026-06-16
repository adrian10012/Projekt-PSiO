#include "Menu.h"
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

Menu::Menu(sf::Vector2f pos, sf::Vector2f sz, sf::Font& f)
    : position(pos), size(sz), font(f), is_open(true)
{
    bgTex.loadFromFile("textures/WoodTable.png");
    btnTex.loadFromFile("textures/BigBlueButton_Regular.png");

    bgBounds = sf::FloatRect(pos.x, pos.y, sz.x, sz.y);

    createTile("Ekwipunek", 0);
    createTile("Platnerz", 1);
    createTile("Wiedzma", 2);
    createTile("Kowal", 3);
}

void Menu::createTile(const std::string& name, int index) {
    Tile t;
    t.name = name;

    float width = 110.f;
    float height = 60.f;
    float spacing = 10.f;
    float startX = position.x + (size.x - (4 * width + 3 * spacing)) / 2.f;
    float y = position.y + size.y / 2.f - height / 2.f;
    float x = startX + index * (width + spacing);

    t.bounds = sf::FloatRect(x, y, width, height);

    t.label.setFont(font);
    t.label.setString(name);
    t.label.setCharacterSize(18);
    t.label.setFillColor(sf::Color::White);

    sf::FloatRect r = t.label.getLocalBounds();
    t.label.setOrigin(r.width / 2.f, r.height / 2.f);
    t.label.setPosition(x + width / 2.f, y + height / 2.f);

    tiles.push_back(t);
}

void Menu::draw(sf::RenderWindow& window) {
    if (!is_open) return;
    drawSmartUI(window, bgTex, bgBounds.left, bgBounds.top, bgBounds.width, bgBounds.height);
    for (auto& t : tiles) {
        drawSmartUI(window, btnTex, t.bounds.left, t.bounds.top, t.bounds.width, t.bounds.height);
        window.draw(t.label);
    }
}

std::string Menu::handleClick(sf::Vector2i mouse) {
    if (!is_open) return "";
    for (const auto& t : tiles) {
        if (t.bounds.contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y))) {
            return t.name;
        }
    }
    return "";
}

bool Menu::open() const { return is_open; }