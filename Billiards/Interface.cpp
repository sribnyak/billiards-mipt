#include "Interface.h"

namespace settings {

real fps = 60;
sf::Color tableColor{0x00, 0x88, 0x3e};
sf::Color ballColor{0x74, 0x00, 0x14};
sf::Color borderColor{0x54, 0x54, 0x54};
sf::Color cueColor{0xbe, 0xb0, 0x62};
real force_factor = 3;
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
                 "Billiards", sf::Style::Close),
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
        handleEvents();
        time = clock.getElapsedTime().asSeconds();
        if (time >= 1 / settings::fps)
            break;
    }
    time = 1 / settings::fps;
    clock.restart();
}

void Interface::handleEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed ||
                event.type == sf::Event::KeyPressed &&
                        event.key.code == sf::Keyboard::Escape) {
            kill();
        }
    }
}

void Interface::simulate() {
    while (isAlive() && table.ballsMoving()) {
        demonstrate();
        table.simulate(time);
    }
}

Vector2 Interface::getStrikeVelocity() {
    Vector2 theBall = settings::transform(table.balls[0].position);
    Vector2 mouse;
    cueImage.onTable = true;
    cueImage.setPosition(theBall);
    while (isAlive()) {
        mouse = vector_cast<real>(sf::Mouse::getPosition(window));
        cueImage.setDirection(theBall - mouse);
        demonstrate();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            break;
        }
    }
    cueImage.onTable = false;
    return settings::force_factor * (mouse - theBall) / settings::scale;
}

bool Interface::restartAtTheEnd() {
    int cnt = static_cast<int>(settings::fps);
    while (isAlive() && --cnt) {
        demonstrate();
    }
    return true;
}

void Interface::kill() {
    window.close();
}

bool Interface::isAlive() {
    return window.isOpen();
}

CueImage::CueImage() : shape(4), onTable(false) {
    auto unit = Ball::radius * settings::scale / 3;
    shape.setPoint(0, sf::Vector2f(0, -unit / 2));
    shape.setPoint(1, sf::Vector2f(sizes::cueLength * settings::scale, -unit));
    shape.setPoint(2, sf::Vector2f(sizes::cueLength * settings::scale, unit));
    shape.setPoint(3, sf::Vector2f(0, unit / 2));
    shape.setOrigin(-Ball::radius * settings::scale * 1.5f, 0);
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
        : borders(Vector2(sizes::tableWidth + 2 * sizes::borderWidth,
                          sizes::tableHeight + 2 * sizes::borderWidth)
                          * settings::scale),
          surface(Vector2(sizes::tableWidth, sizes::tableHeight)
                          * settings::scale) {
    borders.setPosition(settings::transform(Vector2(-sizes::borderWidth,
                                                    -sizes::borderWidth)));
    surface.setPosition(settings::transform(Vector2(0, 0)));
    borders.setFillColor(settings::borderColor);
    surface.setFillColor(settings::tableColor);
    createCentralPockets();
    createCornerPockets();
}

void TableImage::createCentralPockets() {
    sf::RectangleShape pocket(
            Vector2(sizes::centralPocketWidth,
                    sizes::borderWidth * 2) * settings::scale);
    pocket.setFillColor(sf::Color::Black);
    pocket.setOrigin(settings::scale * Vector2(sizes::centralPocketWidth / 2,
                                               0));
    pocket.setPosition(settings::transform(Vector2(sizes::tableWidth / 2,
                                                   -sizes::borderWidth * 2)));
    pockets.push_back(pocket);
    pocket.setPosition(settings::transform(Vector2(sizes::tableWidth / 2,
                                                   sizes::tableHeight)));
    pockets.push_back(pocket);
}

void TableImage::createCornerPockets() {
    sf::RectangleShape pocket(
            Vector2(sizes::cornerPocketWidth,
                    sizes::borderWidth * 3) * settings::scale);
    pocket.setOrigin(settings::scale * Vector2(sizes::cornerPocketWidth / 2,
                                               0));
    pocket.setFillColor(sf::Color::Black);
    real offset = sizes::centralPocketWidth / std::sqrt(2) / 2;

    pocket.setPosition(settings::transform(Vector2(offset, offset)));
    pocket.setRotation(135);
    pockets.push_back(pocket);
    pocket.setPosition(settings::transform(Vector2(sizes::tableWidth - offset,
                                                   offset)));
    pocket.setRotation(-135);
    pockets.push_back(pocket);
    pocket.setPosition(settings::transform(
            Vector2(sizes::tableWidth - offset, sizes::tableHeight - offset)));
    pocket.setRotation(-45);
    pockets.push_back(pocket);
    pocket.setPosition(settings::transform(
            Vector2(offset, sizes::tableHeight - offset)));
    pocket.setRotation(45);
    pockets.push_back(pocket);
}

void TableImage::draw(sf::RenderTarget& target,
                      sf::RenderStates states) const {
    target.draw(borders, states);
    target.draw(surface, states);
    for (const auto& pocket : pockets) {
        target.draw(pocket, states);
    }
}
