#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({500, 500}), "My window");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
