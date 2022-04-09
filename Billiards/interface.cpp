#include "interface.h"

void Interface::Settings::change() {
    // TODO: implement
}

void Interface::drawBall(const Ball& ball) {
    sf::CircleShape image(ball.radius * settings.scale);
    image.setOrigin(Vector2(ball.radius * settings.scale,
                            ball.radius * settings.scale));
    image.setPosition(ball.position * settings.scale);
    window.draw(image);
}

void Interface::drawBorder(const Border& border) {
    sf::RectangleShape image;
    image.setSize((border.bottomRight - border.topLeft) * settings.scale);
    image.setPosition(border.topLeft * settings.scale);
    window.draw(image);
}

void Interface::drawScene(const Table& table) {
    for (auto& ball : table.balls) {
        drawBall(ball);
    }
    for (auto border : table.borders) {
        drawBorder(*border);
    }
}

Interface::Interface(Table& table) : settings(), window(
        sf::VideoMode(table.w * settings.scale, table.h * settings.scale),
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
