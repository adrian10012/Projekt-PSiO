#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Player;

class Wiedzma {
public:
    Wiedzma(sf::Font& font, Player* p);

    void draw(sf::RenderWindow& window);
    void toggle();
    bool isOpen() const;
    void handleClick(sf::Vector2i mouse);

private:
    struct Slot {
        sf::FloatRect bounds;
        sf::Text label;
        sf::Sprite itemIcon; // Dodano sprite do przechowywania ikony
    };

    void createSlot(Slot& slot, const std::string& name, int col, int row);
    void drawSlot(sf::RenderWindow& window, Slot& slot);

    sf::Font& font;
    Player* player;
    bool open;

    sf::Texture bgTex;
    sf::Texture closeTex;
    sf::Texture slotTex;
    sf::Texture potionsTex; // Tekstura potek

    sf::FloatRect bgBounds;
    sf::FloatRect closeBounds;

    Slot pot_hp_small, pot_hp_big;
    Slot pot_rhp_small, pot_rhp_big;
    Slot pot_str_small, pot_str_big;
    Slot pot_arm_small, pot_arm_big;
};