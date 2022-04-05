#include "objects.h"
#include "physics.h"

namespace tableSettings {
    real h = 1.27 + 2 * borderWidth;
    real w = 2.54 + 2 * borderWidth;
    real frictionAcceleration = 0.07;
}

Ball::Ball(const Vec2r& position, const Vec2r& velocity) // default velocity 0
    : position(position), velocity(velocity),
      pocketed(false), image(radius * scale) {
    image.setOrigin(Vec2r(radius * scale, radius * scale));
    image.setPosition(position * scale);
}

void Ball::move(real t) {
    position += t * velocity;
    image.setPosition(position * scale);
    if (position.x < -radius || position.x > tableSettings::w + radius ||
            position.y < -radius || position.y > tableSettings::h + radius) {
        pocketed = true;
        ++pocketedBalls;
        --aliveBalls;
        velocity = Vec2r(0, 0);
    }
}

Border::Border(const Vec2r& topLeft, const Vec2r& bottomRight)
    : topLeft(topLeft), bottomRight(bottomRight) {
    Vec2r diag = bottomRight - topLeft;
    image.setSize(diag * scale);
    image.setPosition(topLeft * scale);
}

VerticalBorder::VerticalBorder(const Vec2r& topLeft, const Vec2r& bottomRight)
    : Border(topLeft, bottomRight) {
    face = (left() > tableSettings::w / 2 ? left() : right());
}
HorizontalBorder::HorizontalBorder(const Vec2r& topLeft, const Vec2r& bottomRight)
    : Border(topLeft, bottomRight) {
    face = (top() > tableSettings::h / 2 ? top() : bottom());
}


Graphics::Graphics() {
    window = window(
            sf::VideoMode(tableSettings::w * scale, tableSettings::h * scale),
            "Billiards"
    );
}

template <typename T>
void Graphics::drawObject(T &obj) {
    window.draw(obj.image);
}

void Graphics::drawBalls(vector<Ball> &balls) {
    for (auto& ball : balls)
        if (!ball.pocketed) {
            drawObject(ball);
        }
}

template <typename T>
void Graphics::drawBorders(vector<T> &borders) {
    for (auto& border : borders) {
        drawObject(border);
    }
}


Settings::Settings() {
    scale = 300;
    fps = 30;
}

void Settings::change() {
    //to_do
}


void Table::createBalls() {
    real yc = tableSettings::h / 2.0f;
    real xf = yc;
    real xb = tabtableSettingsle::w - xf;
    real dx = std::sqrt(3) * Ball::radius * 1.01f;
    real dy = Ball::radius * 1.01f;
    balls.emplace_back(Vec2r(xf, yc));
    balls.emplace_back(Vec2r(xb, yc));
    balls.emplace_back(Vec2r(xb + dx, yc - dy));
    balls.emplace_back(Vec2r(xb + dx, yc + dy));
    balls.emplace_back(Vec2r(xb + 2 * dx, yc - 2 * dy));
    balls.emplace_back(Vec2r(xb + 2 * dx, yc));
    balls.emplace_back(Vec2r(xb + 2 * dx, yc + 2 * dy));
    aliveBalls = balls.size();
    pocketedBalls = 0;
}

void Table::createBorders() {
    real center = tableSettings::w / 2.f;
    real centerOffset = 1.4 * Ball::radius;
    real cornerOffset = borderWidth + 2 * Ball::radius;

    verticalBorders.emplace_back(
            Vec2r(0, cornerOffset),
            Vec2r(borderWidth, tableSettings::h - cornerOffset));
    verticalBorders.emplace_back(
            Vec2r(tableSettings::w - borderWidth, cornerOffset),
            Vec2r(tableSettings::w, tableSettings::h - cornerOffset));

    horizontalBorders.emplace_back(
            Vec2r(cornerOffset, 0),
            Vec2r(center - centerOffset, borderWidth));
    horizontalBorders.emplace_back(
            Vec2r(center + centerOffset, 0),
            Vec2r(tableSettings::w - cornerOffset, borderWidth));
    horizontalBorders.emplace_back(
            Vec2r(cornerOffset, tableSettings::h - borderWidth),
            Vec2r(center - centerOffset, tableSettings::h));
    horizontalBorders.emplace_back(
            Vec2r(center + centerOffset, tableSettings::h - borderWidth),
            Vec2r(tableSettings::w - cornerOffset, tableSettings::h));
}

bool Table::ballsStopped() {
    for (auto& ball : balls)
        if (!ball.pocketed) {
            if (ball.velocity.x != 0 || ball.velocity.y != 0) {
                return false;
            }
        }
    return true;
}

Table::Table() {
    graphics = Graphics();
    borderWidth = 0.05;
    createBalls();
    createBorders();
}

void Table::drawScene() {
    graphics.drawObjects(balls);
    graphics.drawObjects(verticalBorders);
    graphics.drawObjects(horizontalBorders);
}

Game::Game() {
    table = Table();
    settings = Settings();
    state = GameState::strike;
}

void Game::mainLoop() {
    table.graphics.window.setVerticalSyncEnabled(true);
    sf::Clock clock;
    float time;
    while (table.graphics.window.isOpen()) {
        table.graphics.window.clear();
        table.drawScene();
        table.graphics.window.display();

        if (state == GameState::simulation) {
            time = clock.getElapsedTime().asSeconds();
            if (time < 1 / fps) continue;
            time = 1 / fps;
            clock.restart();

            simulate(time);

            if (table.ballsStopped()) {
                state = (table.balls[0].pocketed || aliveBalls == 0
                         ? GameState::end : GameState::strike);
            }

            sf::Event event;
            while (table.graphics.window.pollEvent(event)) {
                if (event.type == sf::Event::Closed ||
                    event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Escape) {
                    table.graphics.window.close();
                }
            }
        } else if (state == GameState::strike) {
            real x, y;
            std::cout << "Give a velocity to the ball\n";
            std::cout << "x: " << std::flush;
            std::cin >> x;
            std::cout << "y: " << std::flush;
            std::cin >> y;
            table.balls[0].velocity.x = x;
            table.balls[0].velocity.y = y;
            state = GameState::simulation;
            clock.restart();
        } else if (state == GameState::end) {
            std::cout << "Game over" << std::endl;
            break;
        }
    }
}