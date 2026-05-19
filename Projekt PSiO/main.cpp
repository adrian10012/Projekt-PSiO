#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Udalo sie zainstalowac SFML!");
    sf::CircleShape shape(150.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(325.f, 225.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    return 0;
}