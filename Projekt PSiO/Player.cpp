#include "Player.h"

Player::Player() {
    shape.setRadius(16.f);
    shape.setOrigin(16.f, 16.f);
    shape.setFillColor(sf::Color::Green);
    pos = sf::Vector2f(400.f, 300.f);
    speed = 250.f;
}

void Player::update(float dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) pos.y -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) pos.y += speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) pos.x -= speed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) pos.x += speed * dt;
    shape.setPosition(pos);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}