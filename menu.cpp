#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

class Menu {
public:
    Menu(sf::Vector2f pos, sf::Vector2f size, sf::Font& font)
        : position(pos), size(size), is_open(true), font(font)
    {
        background.setSize(size);
        background.setPosition(pos);
        background.setFillColor(sf::Color(50, 50, 50, 230));

        close_button.setSize({ 40, 40 });
        close_button.setFillColor(sf::Color(180, 40, 40));
        close_button.setPosition(pos.x + size.x - 50, pos.y + 10);

        createTile("Ekwipunek", 0);
        createTile("Kowal", 1);
        createTile("Platnerz", 2);
        createTile("Wiedzma", 3);
    }

    void draw(sf::RenderWindow& window) {
        if (!is_open) return;

        window.draw(background);
        window.draw(close_button);

        for (auto& t : tiles)
        {
            window.draw(t.rect);
            window.draw(t.label);
        }
    }

    void handleClick(sf::Vector2i mouse) {
        if (!is_open) return;

        if (close_button.getGlobalBounds().contains(mouse.x, mouse.y)) {
            is_open = false;
            return;
        }

        for (int i = 0; i < tiles.size(); i++) {
            if (tiles[i].rect.getGlobalBounds().contains(mouse.x, mouse.y)) {
                last_clicked = tiles[i].name;
                std::cout << "Kliknieto: " << last_clicked << std::endl;
            }
        }
    }

    bool open() const { return is_open; }

private:
    struct Tile {
        sf::RectangleShape rect;
        sf::Text label;
        std::string name;
    };

    sf::Vector2f position;
    sf::Vector2f size;

    sf::RectangleShape background;
    sf::RectangleShape close_button;

    sf::Font& font;

    std::vector<Tile> tiles;
    bool is_open;
    std::string last_clicked = "";

    void createTile(const std::string& name, int index) {
        Tile t;
        t.name = name;

        float w = size.x / 2 - 30;
        float h = size.y / 2 - 60;

        float x = position.x + 20 + (index % 2) * (w + 20);
        float y = position.y + 70 + (index / 2) * (h + 20);

        t.rect.setSize({ w, h });
        t.rect.setPosition(x, y);
        t.rect.setFillColor(sf::Color(100, 100, 100));

        t.label.setFont(font);
        t.label.setString(name);
        t.label.setCharacterSize(24);
        t.label.setFillColor(sf::Color::White);

        sf::FloatRect r = t.label.getLocalBounds();
        t.label.setOrigin(r.width / 2, r.height / 2);
        t.label.setPosition(x + w / 2, y + h / 2);

        tiles.push_back(t);
    }
};