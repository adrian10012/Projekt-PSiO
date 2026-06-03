#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Ekwipunek {
public:
    Ekwipunek(sf::Font& font);

    void draw(sf::RenderWindow& window);
    void toggle();
    bool isOpen() const;
    void handleClick(sf::Vector2i mouse);

private:
    struct Slot {
        sf::RectangleShape rect;
        sf::Text label;
    };

    void createSlot(Slot& slot, const std::string& name, int col, int row);
    void drawSlot(sf::RenderWindow& window, Slot& slot);

    sf::Font& font;
    bool open;

    sf::RectangleShape background;
    sf::RectangleShape closeButton;

    Slot helm, chestplate, spodnie, buty;
    Slot miecz, eliksir;
    Slot extra1, extra2, extra3, extra4, extra5, extra6;
};
