#include "Interface.h"

namespace settings {

real fps = 60;
sf::Color tableColor{0x00, 0x88, 0x3e};
sf::Color ballColor{0x74, 0x00, 0x14};
sf::Color borderColor{0x55, 0x2c, 0x0e};
sf::Color cueColor{0xbe, 0xb0, 0x62};
real scale = 400;
const Vector2 origin{2 * sizes::borderWidth * scale,
                     2 * sizes::borderWidth * scale};
Vector2 transform(const Vector2& vector) { return origin + vector * scale; }
unsigned int windowWidth = static_cast<unsigned int>(
        sizes::tableWidth * scale + 2 * origin.x);
unsigned int windowHeight = static_cast<unsigned int>(
        sizes::tableHeight * scale + 2 * origin.y);

} // namespace settings

void Interface::createBallImages(const std::vector<Ball>& balls) {
    auto scaledRadius = Ball::radius * settings::scale;
    ballImages.assign(balls.size(), sf::CircleShape(scaledRadius));
    for (auto& ballImage : ballImages) {
        ballImage.setOrigin(scaledRadius, scaledRadius);
    }
    ballImages[0].setFillColor(settings::ballColor);
}

void Interface::drawBall(const Ball& ball) {
    auto& ballImage = ballImages[ball.id];
    ballImage.setPosition(settings::transform(ball.position));
    window.draw(ballImage);
}

void Interface::drawScene() {
    window.draw(tableImage);
    for (auto& ball : table.balls) {
        drawBall(ball);
    }
    if (cueImage.onTable)
        window.draw(cueImage);
}

Interface::Interface(Table& table)
        : window(sf::VideoMode(settings::windowWidth, settings::windowHeight),
                 "Billiards"),
          table(table), cueImage(), tableImage() {
    window.setVerticalSyncEnabled(true);
    createBallImages(table.balls);
}

Interface::~Interface() {
    if (isAlive()) kill();
}

void Interface::demonstrate() {
    while (isAlive()) {
        window.clear();
        drawScene();
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
        if (time >= 1 / settings::fps)
            break;
    }
    time = 1 / settings::fps;
    clock.restart();
}

void Interface::simulate() {
    while (isAlive() && table.ballsMoving()) {
        demonstrate();
        table.simulate(time);
    }
}

Vector2 Interface::getStrikeVelocity() {
    auto origin = table.balls[0].position * settings::scale;
    cueImage.setPosition(settings::transform(table.balls[0].position));
    cueImage.onTable = true;
    while (isAlive()) { // TODO cycle
        auto mousePosition = sf::Mouse::getPosition(window);
        Vector2 pos = Vector2(mousePosition.x, mousePosition.y) - settings::origin;
        cueImage.setDirection(origin - pos);
        demonstrate();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            cueImage.onTable = false;
            return 3.f * (pos - origin) / settings::scale;
        }
    }
    return {};
}

void Interface::showGameResult() {
    // TODO: implement
    int cnt = 2 * settings::fps;
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

CueImage::CueImage() : shape(4), onTable(false) {
    auto unit = Ball::radius * settings::scale / 3;
    auto dist = Ball::radius * settings::scale * 1.5f;
    shape.setPoint(0, sf::Vector2f(dist, -unit / 2));
    shape.setPoint(1, sf::Vector2f(dist + sizes::cueLength * settings::scale, -unit));
    shape.setPoint(2, sf::Vector2f(dist + sizes::cueLength * settings::scale, unit));
    shape.setPoint(3, sf::Vector2f(dist, unit / 2));
    shape.setFillColor(settings::cueColor);
}

void CueImage::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(shape, states);
}

void CueImage::setPosition(const Vector2& position) {
    shape.setPosition(position);
}

void CueImage::setDirection(const Vector2& direction) {
    shape.setRotation(atan2(direction.y, direction.x) / atan(1) * 45);
}

TableImage::TableImage()
        : borders(settings::scale * Vector2(sizes::tableWidth + 2 * sizes::borderWidth,
                                            sizes::tableHeight + 2 * sizes::borderWidth)),
          surface(settings::scale * Vector2(sizes::tableWidth,
                                            sizes::tableHeight)) {
    borders.setPosition(settings::transform(Vector2(-sizes::borderWidth,
                                                    -sizes::borderWidth)));
    surface.setPosition(settings::transform(Vector2(0, 0)));
    borders.setFillColor(settings::borderColor);
    surface.setFillColor(settings::tableColor);
}

void TableImage::draw(sf::RenderTarget& target,
                      sf::RenderStates states) const {
    target.draw(borders, states);
    target.draw(surface, states);
}
