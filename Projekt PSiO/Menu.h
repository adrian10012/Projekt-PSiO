#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
public:
    Menu(sf::Vector2f pos, sf::Vector2f size, sf::Font& font);

    void draw(sf::RenderWindow& window);
    std::string handleClick(sf::Vector2i mouse);
    bool open() const;

private:
    struct Tile {
        sf::FloatRect bounds;
        sf::Text label;
        std::string name;
    };

    void createTile(const std::string& name, int index);

    sf::Vector2f position;
    sf::Vector2f size;

    sf::Texture bgTex;
    sf::Texture btnTex;

    sf::FloatRect bgBounds;
    sf::Font& font;

    std::vector<Tile> tiles;
    bool is_open;
};