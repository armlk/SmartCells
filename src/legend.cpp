#include "legend.hpp"

void drawLegend(sf::RenderWindow& window, const sf::Font& font) {
    const uint8_t brightness = 100;
    const float padding = 20.0f;
    const float boxSize = 20.0f;
    const float spacing = 10.0f;
    const float textOffset = 30.f;

    struct LegendItem {
        sf::Color color;
        std::string label;
    };

    std::vector<LegendItem> items = {
        {{brightness, 0, 0},   "IS"},
        {{0, brightness, 0}, "AND"},
        {{brightness, 0, brightness}, "NOT"},
        {{brightness, brightness, 0}, "OR"}
    };

    float legendHeight = items.size() * (boxSize + spacing) - spacing;
    float legendWidth = 80.0f;

    sf::Vector2u winSize = window.getSize();
    float startX = winSize.x - legendWidth - padding;
    float startY = winSize.y - legendHeight - padding;

    for (size_t i = 0; i < items.size(); i++) {
        float y = startY + i * (boxSize + spacing);

        sf::RectangleShape rect(sf::Vector2f(boxSize, boxSize));
        rect.setFillColor(items[i].color);
        rect.setPosition({startX, y});

        sf::Text text(font, items[i].label);
        text.setFillColor(sf::Color::White);
        text.setPosition({startX + textOffset, y - 2});

        window.draw(rect);
        window.draw(text);
    }
}
