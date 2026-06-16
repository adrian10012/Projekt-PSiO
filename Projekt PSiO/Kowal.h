#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Player;

class Kowal {
public:
    Kowal(sf::Font& font, Player* p);

    void draw(sf::RenderWindow& window);
    void toggle();
    bool isOpen() const;
    void handleClick(sf::Vector2i mouse);

private:
    struct Slot {
        sf::FloatRect bounds;
        sf::Text label;
    };

    void createSlot(Slot& slot, const std::string& name, int col, int row);
    void drawSlot(sf::RenderWindow& window, Slot& slot);

    sf::Font& font;
    Player* player;
    bool open;

    sf::Texture bgTex;
    sf::Texture closeTex;
    sf::Texture slotTex;

    sf::FloatRect bgBounds;
    sf::FloatRect closeBounds;

    Slot sword1, sword2, sword3, sword4;
};