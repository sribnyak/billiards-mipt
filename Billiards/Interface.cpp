#include "Interface.h"

void Interface::Settings::change() {
    // TODO: implement
}

void Interface::createBallImages(const std::vector<Ball>& balls) {
    auto scaledRadius = Ball::radius * settings.scale;
    ballImages.assign(balls.size(), sf::CircleShape(scaledRadius));
    for (auto& ballImage : ballImages) {
        ballImage.setOrigin(Vector2(scaledRadius, scaledRadius));
    }
    ballImages[0].setFillColor(settings.ballColor);
}

void Interface::drawBall(const Ball& ball) {
    auto& ballImage = ballImages[ball.id];
    ballImage.setPosition(ball.position * settings.scale);
    window.draw(ballImage);
}

void Interface::drawBorder(const Border& border) {
    sf::RectangleShape image;
    image.setSize((border.bottomRight - border.topLeft) * settings.scale);
    image.setPosition(border.topLeft * settings.scale);
    image.setFillColor(settings.borderColor);
    window.draw(image);
}

void Interface::drawScene() {
    for (auto& ball : table.balls) {
        drawBall(ball);
    }
    for (const auto& border : table.borders) {
        drawBorder(*border);
    }
}

Interface::Interface(Table& table)
        : settings(), window(sf::VideoMode(table.w * settings.scale,
                                           table.h * settings.scale),
                             "Billiards"),
          table(table),
          cueImage(settings.scale, settings.cueColor) {
    window.setVerticalSyncEnabled(true);
    createBallImages(table.balls);
}

Interface::~Interface() {
    if (isAlive()) kill();
}

void Interface::demonstrate() {
    while (isAlive()) {
        window.clear(settings.tableColor);
        drawScene();
        if (cueImage.onTable)
            cueImage.draw(window);
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                    event.type == sf::Event::KeyPressed &&
                            event.key.code == sf::Keyboard::Escape) {
                kill();
            }
        }
        time = clock.getElapsedTime().asSeconds();
        if (time >= 1 / settings.fps)
            break;
    }
    time = 1 / settings.fps;
    clock.restart();
}

void Interface::simulate() {
    while (isAlive() && !table.ballsStopped()) {
        demonstrate();
        table.simulate(time);
    }
}

Vector2 Interface::getStrikeVelocity() {
    auto origin = table.balls[0].position * settings.scale;
    cueImage.setPosition(origin);
    cueImage.onTable = true;
    while (isAlive()) {
        auto mousePosition = sf::Mouse::getPosition(window);
        Vector2 pos(mousePosition.x, mousePosition.y);
        cueImage.setDirection(pos - origin);
        demonstrate();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            cueImage.onTable = false;
            return 5.f * (origin - pos) / settings.scale;
        }
    }
    return {};
}

void Interface::showGameResult() {
    // TODO: implement
    int cnt = 2 * settings.fps;
    while (isAlive() && --cnt) {
        demonstrate();
    }
}

void Interface::kill() {
    window.close();
}

bool Interface::isAlive() {
    return window.isOpen();
}

CueImage::CueImage(real scale, sf::Color color) : shape(4), onTable(false) {
    auto unit = Ball::radius * scale;
    shape.setPoint(0, sf::Vector2f(unit * 1.5f, -unit / 7));
    shape.setPoint(1, sf::Vector2f(unit * 30, -unit / 5));
    shape.setPoint(2, sf::Vector2f(unit * 30, unit / 5));
    shape.setPoint(3, sf::Vector2f(unit * 1.5f, unit / 7));
    shape.setFillColor(color);
}

void CueImage::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void CueImage::setPosition(const Vector2& position) {
    shape.setPosition(position);
}

void CueImage::setDirection(const Vector2& direction) {
    shape.setRotation(atan2(direction.y, direction.x) / atan(1) * 45);
}
