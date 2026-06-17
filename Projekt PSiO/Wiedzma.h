#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Player;

class Wiedzma {
public:
    struct Slot {
        sf::FloatRect bounds;
        sf::Text label;
        sf::Text priceLabel; // CENA
        sf::Sprite itemIcon;
        int price; // CENA
    };

    Wiedzma(sf::Font& font, Player* p);

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
    sf::Texture potionsTex;

    sf::FloatRect bgBounds;
    sf::FloatRect closeBounds;

    Slot pot_hp_small, pot_hp_big, pot_rhp_small, pot_rhp_big, pot_str_small, pot_str_big, pot_arm_small, pot_arm_big;

    void createSlot(Slot& slot, const std::string& name, int col, int row, int price);
    void drawSlot(sf::RenderWindow& window, Slot& slot);
};