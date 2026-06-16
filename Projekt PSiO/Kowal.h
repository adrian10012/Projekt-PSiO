#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Player;

class Kowal {
public:
    struct Slot {
        sf::FloatRect bounds;
        sf::Text label;
        sf::Sprite itemIcon;
    };

    Kowal(sf::Font& font, Player* p);

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

    Slot sword1, sword2, sword3, sword4;

    void createSlot(Slot& slot, const std::string& name, int col, int row, const std::string& texPath);
    void drawSlot(sf::RenderWindow& window, Slot& slot);
};