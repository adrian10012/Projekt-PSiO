#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
public:
    Menu(sf::Vector2f pos, sf::Vector2f size, sf::Font& font);

    void draw(sf::RenderWindow& window);
    void handleClick(sf::Vector2i mouse);

    std::string getLastClicked() const;
    bool open() const;

private:
    struct Tile {
        sf::RectangleShape rect;
        sf::Text label;
        std::string name;
    };

    void createTile(const std::string& name, int index);

    sf::Vector2f position;
    sf::Vector2f size;

    sf::RectangleShape background;
    sf::RectangleShape close_button;

    sf::Font& font;

    std::vector<Tile> tiles;
    bool is_open;
    std::string last_clicked;
};
