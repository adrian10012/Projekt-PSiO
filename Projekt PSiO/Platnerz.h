#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Player;

class Platnerz {
public:
    struct Slot {
        sf::FloatRect bounds;
        sf::Text label;
        sf::Sprite itemIcon;
    };

    Platnerz(sf::Font& font, Player* p);

    void draw(sf::RenderWindow& window);
    void toggle();
    bool isOpen() const;
    void handleClick(sf::Vector2i mouse);

private:
    sf::Font& font;
    Player* player;
    bool open;

    sf::Texture bgTex;
    sf::Texture closeTex;
    sf::Texture slotTex;

    sf::FloatRect bgBounds;
    sf::FloatRect closeBounds;

    Slot helm1, helm2;
    Slot chest1, chest2;
    Slot buty1, buty2;
    Slot spodnie1, spodnie2;

    void createSlot(Slot& slot, const std::string& name, int col, int row, const std::string& texPath);
    void drawSlot(sf::RenderWindow& window, Slot& slot);
};