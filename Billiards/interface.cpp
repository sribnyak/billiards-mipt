#include "interface.h"

template <typename T>
void Interface::drawObject(T &obj) {
    window.draw(obj.image);
}

void Interface::drawScene(const Table& table) {
    for (auto& ball : table.balls)
        if (!ball.pocketed) {
            drawObject(ball);
        }
    for (auto& border : table.verticalBorders) {
        drawObject(border);
    }
    for (auto& border : table.horizontalBorders) {
        drawObject(border);
    }
}

Interface::Interface() : window(
        sf::VideoMode(Table::w * Settings::scale, Table::h * Settings::scale),
        "Billiards") {
    window.setVerticalSyncEnabled(true);
}

Interface::~Interface() {
    if (!isAlive()) kill();
}

void Interface::demonstrate(Table& table) {
    window.clear();
    drawScene(table);
    window.display();

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed ||
            event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape) {
            kill();
        }
    }
}
Vector2 Interface::getStrikeVelocity() {
    real x, y;
    std::cout << "Give a velocity to the ball\n";
    std::cout << "x: " << std::flush;
    std::cin >> x;
    std::cout << "y: " << std::flush;
    std::cin >> y;
    return Vector2(x, y);
}
void Interface::showGameResult() {
    std::cout << "Game over" << std::endl;
}
void Interface::kill() {
    window.close();
}
bool Interface::isAlive() {
    return window.isOpen();
}
