#include "legend.hpp"

void drawLegend(sf::RenderWindow& window, const sf::Font& font) {
    const float padding = 20.f;
    const float boxSize = 20.f;
    const float spacing = 10.f;
    const float textOffset = 30.f;

    struct LegendItem {
        sf::Color color;
        std::string label;
    };

    std::vector<LegendItem> items = {
        {sf::Color::Red,   "red meaning"},
        {sf::Color::Green, "green meaning"},
        {sf::Color(255, 105, 180), "pink meaning"}, // Hot pink
        {sf::Color::Yellow, "yellow meaning"}
    };

    float legendHeight = items.size() * (boxSize + spacing) - spacing;
    float legendWidth = 180.f; // Adjust as needed for label length

    sf::Vector2u winSize = window.getSize();
    float startX = winSize.x - legendWidth - padding;
    float startY = winSize.y - legendHeight - padding;

    for (size_t i = 0; i < items.size(); ++i) {
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
