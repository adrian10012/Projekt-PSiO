#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Ekwipunek.h"
#include "Platnerz.h"
#include "Wiedzma.h"
#include "Kowal.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Jarek Kaczynski");
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("OpenSans-SemiBold.ttf");

    Menu menu({ 150, 80 }, { 600, 400 }, font);
    Ekwipunek ekwipunek(font);
    Platnerz platnerz(font);
    Wiedzma wiedzma(font);
    Kowal kowal(font);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mouse = sf::Mouse::getPosition(window);

                // Klikniêcia w menu
                if (ekwipunek.isOpen()) {
                    ekwipunek.handleClick(mouse);
                }
                else if (platnerz.isOpen()) {
                    platnerz.handleClick(mouse);
                }
                else if (wiedzma.isOpen()) {
                    wiedzma.handleClick(mouse);
                }
                else if (kowal.isOpen()) {
                    kowal.handleClick(mouse);
                }
                else {
                    menu.handleClick(mouse);

                    if (menu.getLastClicked() == "Ekwipunek")
                        ekwipunek.toggle();
                    if (menu.getLastClicked() == "Platnerz")
                        platnerz.toggle();
                    if (menu.getLastClicked() == "Wiedzma")
                        wiedzma.toggle();
                    if (menu.getLastClicked() == "Kowal")
                        kowal.toggle();
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));

        // Rysowanie
        if (menu.open()) menu.draw(window);
        if (ekwipunek.isOpen()) ekwipunek.draw(window);
        if (platnerz.isOpen()) platnerz.draw(window);
        if (wiedzma.isOpen()) wiedzma.draw(window);
        if (kowal.isOpen()) kowal.draw(window);

        window.display();
    }

    return 0;
}