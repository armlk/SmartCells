#include "arrow.hpp"
#include "legend.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window.hpp>
#include <deque>
#include <iostream>
#include <random>

std::pair<int, int> offsets[9] = {{0, 0}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0},
                                  {1, 0}, {-1, 1},  {0, 1},  {1, 1}};
const size_t SIZE = 1000;
const size_t GRID_SIZE = 5;
const size_t CELL_SIZE = SIZE / GRID_SIZE;
const size_t INITIAL_SIZE = 3;
const size_t BRIGHTNESS = 100;
const bool DRAW_ARROWS = true;
const float ARROW_THICKNESS = 2.0f;
const float ARROW_LENGTH = 5.0f;
bool plau = false;

enum CellType { INACTIVE, IS, AND, NOT, OR };

struct Cell {
    sf::RectangleShape rect;
    CellType type;
    int input1;
    int input2;
    bool state = false;
};

void drawCircleIfActive(sf::RenderWindow &w, Cell &c) {
    if (c.state) {
        sf::CircleShape circle(CELL_SIZE / 5.0f);
        circle.setFillColor(sf::Color::White);
        circle.setOrigin({circle.getRadius(), circle.getRadius()});
        circle.setPosition({c.rect.getPosition().x + CELL_SIZE / 2.0f,
                            c.rect.getPosition().y + CELL_SIZE / 2.0f});
        w.draw(circle);
    }
}

void drawInputArrow(sf::RenderWindow &w, float x, float y, size_t offsetIndex) {
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

void drawCanvas(sf::RenderWindow &window, std::deque<std::deque<Cell>> &cells) {
    for (size_t i = 0; i < cells.size(); i++) {
        for (size_t j = 0; j < cells[i].size(); j++) {
            if (cells[i][j].type != CellType::INACTIVE) {
                window.draw(cells[i][j].rect);
                drawInputArrow(window, cells[i][j].rect.getPosition().x,
                               cells[i][j].rect.getPosition().y,
                               cells[i][j].input1);

                if (cells[i][j].type != CellType::NOT &&
                    cells[i][j].type != CellType::IS) {
                    drawInputArrow(window, cells[i][j].rect.getPosition().x,
                                   cells[i][j].rect.getPosition().y,
                                   cells[i][j].input2);
                }

                drawCircleIfActive(window, cells[i][j]);
            }
        }
    }

    const sf::Font font("JetBrainsMonoNerdFont-Medium.ttf");
    drawLegend(window, font);
}

int main() {
    std::random_device rd;
    std::mt19937 seeded_generator(rd());
    std::uniform_int_distribution<> d1(0, 4);
    std::uniform_int_distribution<> d2(0, 8);
    std::deque<std::deque<Cell>> cells(GRID_SIZE, std::deque<Cell>(GRID_SIZE));

    for (size_t i = 0; i < cells.size(); i++) {
        for (size_t j = 0; j < cells[i].size(); j++) {
            cells[i][j].rect.setPosition({j * static_cast<float>(CELL_SIZE),
                                          i * static_cast<float>(CELL_SIZE)});

            size_t start = GRID_SIZE / 2 - INITIAL_SIZE / 2;
            size_t end = start + INITIAL_SIZE;
            if (i >= start && i < end && j >= start && j < end) {
                cells[i][j].type = static_cast<CellType>(d1(seeded_generator));
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

            if (event->is<sf::Event::KeyPressed>() &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                while (true) {
                    size_t x, y, choice, mode, input, inputChoice;
                    bool state;
                    std::cout
                        << "Enter the grid coordinate of the cell you'd like "
                           "to change space-separated: ";
                    std::cin >> x >> y;
                    while (x >= GRID_SIZE || y >= GRID_SIZE) {
                        std::cout
                            << "Invalid input. Please try again." << std::endl
                            << "Enter the grid coordinate of the cell you'd "
                               "like to change space-separated: ";
                        std::cin >> x >> y;
                    }

                    window.clear();
                    cells[y][x].rect.setOutlineColor({sf::Color::White});
                    cells[y][x].rect.setOutlineThickness(-5.0f);
                    drawCanvas(window, cells);
                    window.display();

                    std::cout
                        << "What would you like to change (1 for state, 2 "
                           "for mode, 3 for inputs)? ";
                    std::cin >> choice;
                    while (choice != 1 && choice != 2 && choice != 3) {
                        std::cout
                            << "Invalid input. Please try again." << std::endl
                            << "What would you like to change (1 for state, "
                               "2 for mode, 3 for inputs)? ";
                        std::cin >> choice;
                    }

                    switch (choice) {
                    case 1:
                        std::cout << "Enter the new state for cell at (" << x
                                  << ", " << y << ") (0 for off, 1 for on): ";
                        std::cin >> state;
                        while (state != 0 && state != 1) {
                            std::cout << "Invalid input. Please try again."
                                      << std::endl
                                      << "Enter the new state for cell at ("
                                      << x << ", " << y
                                      << ") (0 for off, 1 for on): ";
                            std::cin >> state;
                        }

                        cells[y][x].state = state;
                        std::cout << cells[y][x].state << std::endl;

                        break;

                    case 2:
                        std::cout
                            << "Enter the new state for cell at (" << x << ", "
                            << y
                            << ") (0 for INACTIVE, 1 for IS, 2 for AND, 3 "
                               "for NOT, 4 for OR): ";
                        std::cin >> mode;
                        while (mode != 0 && mode != 1 && mode != 2 &&
                               mode != 3 && mode != 4) {
                            std::cout << "Invalid input. Please try again."
                                      << std::endl
                                      << "Enter the new state for cell at ("
                                      << x << ", " << y
                                      << ") (0 for INACTIVE, 1 for IS, 2 for "
                                         "AND, 3 for "
                                         "NOT, 4 for OR): ";
                            std::cin >> mode;
                        }

                        cells[y][x].type = static_cast<CellType>(mode);
                        break;

                    case 3:
                        std::cout
                            << "Would you like to change input 1 or input 2 "
                               "(1 for 1, 2 for 2)? ";
                        std::cin >> inputChoice;
                        while (inputChoice != 1 && inputChoice != 2) {
                            std::cout << "Invalid input. Please try again."
                                      << std::endl
                                      << "Would you like to change input 1 or "
                                         "input 2 (1 for 1, 2 for 2)? ";
                            std::cin >> inputChoice;
                        }

                        std::cout
                            << "From which input direction do you want to "
                               "take input from (0 for self-reference, 1 for "
                               "top-left, 2 for top, 3 for top-right, 4 for "
                               "left, 5 for right, 6 for bottom-left, 7 for "
                               "bottom, 8 for bottom-right)? ";
                        std::cin >> input;
                        while (input > 8) {
                            std::cout
                                << "Invalid input. Please try again."
                                << std::endl
                                << "From which input direction do you want to "
                                   "take "
                                   "input from (0 for self-reference, 1 for "
                                   "top-left, 2 for top, 3 for top-right, 4 "
                                   "for "
                                   "left, 5 for right, 6 for bottom-left, 7 "
                                   "for "
                                   "bottom, 8 for bottom-right)? ";
                            std::cin >> input;
                        }

                        switch (inputChoice) {
                        case 1:
                            cells[y][x].input1 = input;
                            break;
                        case 2:
                            cells[y][x].input2 = input;
                            break;
                        }

                        break;
                    }

                    window.clear();

                    cells[y][x].rect.setOutlineColor({sf::Color::Transparent});
                    cells[y][x].rect.setOutlineThickness(0.0f);
                    drawCanvas(window, cells);

                    window.display();

                    std::cout << "Would you like to continue making changes (0 for no, 1 for yes)? ";
                    bool cont;
                    std::cin >> cont;
                    if (cont != 1) {
                        break;
                    }
                }
            }
        }

        window.clear();

        for (size_t i = 0; i < cells.size(); i++) {
            for (size_t j = 0; j < cells[i].size(); j++) {
                if (cells[i][j].type != CellType::INACTIVE) {
                    std::pair<int, int> offset1 = offsets[cells[i][j].input1];
                    bool isGround1 =
                        i + offsets[cells[i][j].input1].first >= cells.size() ||
                        j + offsets[cells[i][j].input1].second >=
                            cells[i].size();

                    std::pair<int, int> offset2 = offsets[cells[i][j].input2];
                    bool isGround2 =
                        i + offsets[cells[i][j].input2].first >= cells.size() ||
                        j + offsets[cells[i][j].input2].second >=
                            cells[i].size();

                    bool first =
                        isGround1 ? false
                                  : cells[i + offset1.first][j + offset1.second]
                                        .state;
                    bool second =
                        isGround2 ? false
                                  : cells[i + offset2.first][j + offset2.second]
                                        .state;

                    switch (cells[i][j].type) {
                    case INACTIVE:
                        break;
                    case IS:
                        cells[i][j].state = first;
                        break;
                    case AND:
                        cells[i][j].state = first && second;
                        break;
                    case NOT:
                        cells[i][j].state = !first;
                        break;
                    case OR:
                        cells[i][j].state = first || second;
                        break;
                    }
                }
            }
        }

        plau = !plau;
        sf::CircleShape c(50.0f);
        c.setPosition({plau ? 0.0f : SIZE - 100.0f, 0});
        c.setFillColor(sf::Color::White);
        drawCanvas(window, cells);
        window.draw(c);
        window.display();

        sf::sleep(sf::seconds(1));
    }

    return 0;
}
