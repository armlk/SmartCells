#include "arrow.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <cmath>

void drawArrow(sf::RenderWindow& w, sf::Vector2f initial, sf::Vector2f end, float thickness, float headLength) {
    sf::Vector2f direction = end - initial;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265f;

    // Shaft
    sf::RectangleShape shaft(sf::Vector2f(length - headLength, thickness));
    shaft.setPosition(initial);
    shaft.setFillColor(sf::Color::Black);
    shaft.setOrigin({0, thickness / 2});
    shaft.setRotation(sf::degrees(angle));

    // Arrowhead (triangle)
    sf::ConvexShape head;
    head.setPointCount(3);
    head.setPoint(0, sf::Vector2f(0, 0));
    head.setPoint(1, sf::Vector2f(-headLength, -thickness / 2));
    head.setPoint(2, sf::Vector2f(-headLength, thickness / 2));
    head.setFillColor(sf::Color::Black);

    // Position arrowhead at the end
    head.setPosition(end);
    head.setRotation(sf::degrees(angle));

    w.draw(shaft);
    w.draw(head);
}
