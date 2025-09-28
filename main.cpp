#include "arrow.hpp"
#include "legend.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <deque>

std::pair<int, int> offsets[9] = {{0, 0}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
const size_t SIZE = 1000;
const size_t GRID_SIZE = 10;
const size_t CELL_SIZE = SIZE / GRID_SIZE;
const size_t INITIAL_SIZE = 3;
const size_t BRIGHTNESS = 100;
const bool DRAW_ARROWS = true;
const float ARROW_THICKNESS = 2.0f;
const float ARROW_LENGTH = 5.0f;

enum CellType {
    INACTIVE,
    IS,
    AND,
    NOT,
    OR
};

struct Cell {
    sf::RectangleShape rect;
    CellType type;
    int input1;
    int input2;
    bool state = false;
};

void drawCircleIfActive(sf::RenderWindow& w, Cell& c) {
    if (c.state) {
        sf::CircleShape circle(CELL_SIZE / 5.0f);
        circle.setFillColor(sf::Color::White);
        circle.setOrigin({circle.getRadius(), circle.getRadius()});
        circle.setPosition({
            c.rect.getPosition().x + CELL_SIZE / 2.0f,
            c.rect.getPosition().y + CELL_SIZE / 2.0f
        });
        w.draw(circle);
    }
}

void drawInputArrow(sf::RenderWindow& w, float x, float y, size_t offsetIndex) {
    float startX, startY, endX, endY;
    switch (offsetIndex) {
        case 0:
            return;
        case 1:
            startX = x + CELL_SIZE / 16.0;
            startY = y + CELL_SIZE / 16.0;
            endX = x + CELL_SIZE / 4.0;
            endY = y + CELL_SIZE / 4.0;
            drawArrow(w, sf::Vector2f(startX, startY), sf::Vector2f(endX, endY));
            break;
        case 2:
            startX = x + CELL_SIZE / 2.0;
            startY = y + CELL_SIZE / 16.0;
            endX = x + CELL_SIZE / 2.0;
            endY = y + CELL_SIZE / 4.0;
            drawArrow(w, sf::Vector2f(startX, startY), sf::Vector2f(endX, endY));
            break;
        case 3:
            startX = x + CELL_SIZE * 15.0 / 16.0;
            startY = y + CELL_SIZE * 15.0 / 16.0;
            endX = x + CELL_SIZE * 3.0 / 4.0;
            endY = y + CELL_SIZE * 3.0 / 4.0;
            drawArrow(w, sf::Vector2f(startX, startY), sf::Vector2f(endX, endY));
            break;
        case 4:
            startX = x + CELL_SIZE / 16.0;
            startY = y + CELL_SIZE / 2.0;
            endX = x + CELL_SIZE / 4.0;
            endY = y + CELL_SIZE / 2.0;
            drawArrow(w, sf::Vector2f(startX, startY), sf::Vector2f(endX, endY));
            break;
        case 5:
            startX = x + CELL_SIZE * 15.0 / 16.0;
            startY = y + CELL_SIZE / 2.0;
            endX = x + CELL_SIZE * 3.0 / 4.0;
            endY = y + CELL_SIZE / 2.0;
            drawArrow(w, sf::Vector2f(startX, startY), sf::Vector2f(endX, endY));
            break;
        case 6:
            startX = x + CELL_SIZE / 16.0;
            startY = y + CELL_SIZE * 15.0 / 16.0;
            endX = x + CELL_SIZE / 4.0;
            endY = y + CELL_SIZE * 3.0 / 4.0;
            drawArrow(w, sf::Vector2f(startX, startY), sf::Vector2f(endX, endY));
            break;
        case 7:
            startX = x + CELL_SIZE / 2.0;
            startY = y + CELL_SIZE * 15.0 / 16.0;
            endX = x + CELL_SIZE / 2.0;
            endY = y + CELL_SIZE * 3.0 / 4.0;
            drawArrow(w, sf::Vector2f(startX, startY), sf::Vector2f(endX, endY));
            break;
        case 8:
            startX = x + CELL_SIZE * 15.0 / 16.0;
            startY = y + CELL_SIZE * 15.0 / 16.0;
            endX = x + CELL_SIZE * 3.0 / 4.0;
            endY = y + CELL_SIZE * 3.0 / 4.0;
            drawArrow(w, sf::Vector2f(startX, startY), sf::Vector2f(endX, endY));
            break;
    }
}

int main() {
    std::random_device rd;
    std::mt19937 seeded_generator(rd());
    std::uniform_int_distribution<> d1(0, 4);
    std::uniform_int_distribution<> d2(0, 8);
    std::deque<std::deque<Cell>> cells(GRID_SIZE, std::deque<Cell>(GRID_SIZE));

    for (size_t i = 0; i < cells.size(); i++) {
        for (size_t j = 0; j < cells[i].size(); j++) {
            cells[i][j].rect.setPosition({j * static_cast<float>(CELL_SIZE), i * static_cast<float>(CELL_SIZE)});

            size_t start = GRID_SIZE / 2 - INITIAL_SIZE / 2;
            size_t end = start + INITIAL_SIZE;
            if (i >= start && i < end && j >= start && j < end) {
                cells[i][j].type = static_cast<CellType>(d1(seeded_generator));
                // cells[i][j].type = CellType::NOT;
                cells[i][j].input1 = d2(seeded_generator);
                cells[i][j].input2 = d2(seeded_generator);
                while (cells[i][j].input1 == cells[i][j].input2) {
                    cells[i][j].input2 = d2(seeded_generator);
                }
            } else {
                cells[i][j].type = CellType::INACTIVE;
            }

            switch (cells[i][j].type) {
                case INACTIVE:
                    cells[i][j].rect.setFillColor({0, 0, 0});
                    break;
                case IS:
                    cells[i][j].rect.setFillColor({BRIGHTNESS, 0, 0});
                    break;
                case AND:
                    cells[i][j].rect.setFillColor({0, BRIGHTNESS, 0});
                    break;
                case NOT:
                    cells[i][j].rect.setFillColor({BRIGHTNESS, 0, BRIGHTNESS});
                    break;
                case OR:
                    cells[i][j].rect.setFillColor({BRIGHTNESS, BRIGHTNESS, 0});
                    break;
            }

            cells[i][j].rect.setSize({CELL_SIZE, CELL_SIZE});
        }
    }

    sf::RenderWindow window(sf::VideoMode({SIZE, SIZE}), "My window");


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();

        for (size_t i = 0; i < cells.size(); i++) {
            for (size_t j = 0; j < cells[i].size(); j++) {
                if (cells[i][j].type != CellType::INACTIVE) {
                    std::pair<int, int> offset1 = offsets[cells[i][j].input1];
                    bool isGround1 =                    
                        i + offsets[cells[i][j].input1].first >= cells.size() ||
                        j + offsets[cells[i][j].input1].second >= cells[i].size();

                    std::pair<int, int> offset2 = offsets[cells[i][j].input2];
                    bool isGround2 =
                        i + offsets[cells[i][j].input2].first >= cells.size() ||
                        j + offsets[cells[i][j].input2].second >= cells[i].size();

                    bool first = isGround1 ? false : cells[i + offset1.first][j + offset1.second].state;
                    bool second = isGround2 ? false : cells[i + offset2.first][j + offset2.second].state;

                    window.draw(cells[i][j].rect);
                    drawInputArrow(window, cells[i][j].rect.getPosition().x, cells[i][j].rect.getPosition().y, cells[i][j].input1);
                    
                    if (cells[i][j].type != CellType::NOT && cells[i][j].type != CellType::IS) {
                        drawInputArrow(window, cells[i][j].rect.getPosition().x, cells[i][j].rect.getPosition().y, cells[i][j].input2);
                    }

                    switch (cells[i][j].type) {
                        case INACTIVE:
                            break;
                        case IS:
                            cells[i][j].state = first;
                            drawCircleIfActive(window, cells[i][j]);
                            break;
                        case AND:
                            cells[i][j].state = first && second;
                            drawCircleIfActive(window, cells[i][j]);
                            break;
                        case NOT:
                            cells[i][j].state = !first;
                            drawCircleIfActive(window, cells[i][j]);
                            break;
                        case OR:
                            cells[i][j].state = first || second;
                            drawCircleIfActive(window, cells[i][j]);
                            break;
                    }
                }
                
            }
        }

        const sf::Font font("JetBrainsMonoNerdFont-Medium.ttf");
        drawLegend(window, font);
        window.display();

        sf::sleep(sf::seconds(0.5));
    }

    return 0;
}
