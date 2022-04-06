#include "objects.h"
#include "physics.h"
#include <iostream>

int aliveBalls = 0;
int pocketedBalls = 0;

Ball::Ball(const Vector2& position, const Vector2& velocity) // default velocity 0
    : position(position), velocity(velocity),
      pocketed(false), image(radius * Settings::Settings::scale) {
    image.setOrigin(Vector2(radius * Settings::Settings::scale, radius * Settings::scale));
    image.setPosition(position * Settings::scale);
}

void Ball::move(real t) {
    position += t * velocity;
    image.setPosition(position * Settings::scale);
    if (position.x < -radius || position.x > Table::w + radius ||
            position.y < -radius || position.y > Table::h + radius) {
        pocketed = true;
        ++pocketedBalls;
        --aliveBalls;
        velocity = Vector2(0, 0);
    }
}

Border::Border(const Vector2& topLeft, const Vector2& bottomRight)
    : topLeft(topLeft), bottomRight(bottomRight) {
    Vector2 diag = bottomRight - topLeft;
    image.setSize(diag * Settings::scale);
    image.setPosition(topLeft * Settings::scale);
}

VerticalBorder::VerticalBorder(const Vector2& topLeft, const Vector2& bottomRight)
    : Border(topLeft, bottomRight) {
    face = (left() > Table::w / 2 ? left() : right());
}
HorizontalBorder::HorizontalBorder(const Vector2& topLeft, const Vector2& bottomRight)
    : Border(topLeft, bottomRight) {
    face = (top() > Table::h / 2 ? top() : bottom());
}


Graphics::Graphics()
    : window (
            sf::VideoMode(Table::w * Settings::scale, Table::h * Settings::scale),
            "Billiards"
    )
{}

template <typename T>
void Graphics::drawObject(T &obj) {
    window.draw(obj.image);
}

void Graphics::drawScene(const Table& table) {
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

Settings::Settings() : fps(30) {}

void Settings::change() {
    //to_do
}


void Table::createBalls() {
    real yc = Table::h / 2.0f;
    real xf = yc;
    real xb = Table::w - xf;
    real dx = std::sqrt(3) * Ball::radius * 1.01f;
    real dy = Ball::radius * 1.01f;
    balls.emplace_back(Vector2(xf, yc));
    balls.emplace_back(Vector2(xb, yc));
    balls.emplace_back(Vector2(xb + dx, yc - dy));
    balls.emplace_back(Vector2(xb + dx, yc + dy));
    balls.emplace_back(Vector2(xb + 2 * dx, yc - 2 * dy));
    balls.emplace_back(Vector2(xb + 2 * dx, yc));
    balls.emplace_back(Vector2(xb + 2 * dx, yc + 2 * dy));
    aliveBalls = balls.size();
    pocketedBalls = 0;
}

void Table::createBorders() {
    real center = Table::w / 2.f;
    real centerOffset = 1.4 * Ball::radius;
    real cornerOffset = borderWidth + 2 * Ball::radius;

    verticalBorders.emplace_back(
            Vector2(0, cornerOffset),
            Vector2(borderWidth, Table::h - cornerOffset));
    verticalBorders.emplace_back(
            Vector2(Table::w - borderWidth, cornerOffset),
            Vector2(Table::w, Table::h - cornerOffset));

    horizontalBorders.emplace_back(
            Vector2(cornerOffset, 0),
            Vector2(center - centerOffset, borderWidth));
    horizontalBorders.emplace_back(
            Vector2(center + centerOffset, 0),
            Vector2(Table::w - cornerOffset, borderWidth));
    horizontalBorders.emplace_back(
            Vector2(cornerOffset, Table::h - borderWidth),
            Vector2(center - centerOffset, Table::h));
    horizontalBorders.emplace_back(
            Vector2(center + centerOffset, Table::h - borderWidth),
            Vector2(Table::w - cornerOffset, Table::h));
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
    createBalls();
    createBorders();
}

Game::Game() {
    table = Table();
    settings = Settings();
    state = GameState::strike;
}

void Game::mainLoop() {
    graphics.window.setVerticalSyncEnabled(true);
    sf::Clock clock;
    real time;
    while (graphics.window.isOpen()) {
        graphics.window.clear();
        graphics.drawScene(table);
        graphics.window.display();

        if (state == GameState::simulation) {
            time = clock.getElapsedTime().asSeconds();
            if (time < 1 / settings.fps) continue;
            time = 1 / settings.fps;
            clock.restart();

            simulate(table, time);

            if (table.ballsStopped()) {
                state = (table.balls[0].pocketed || aliveBalls == 0
                         ? GameState::end : GameState::strike);
            }

            sf::Event event;
            while (graphics.window.pollEvent(event)) {
                if (event.type == sf::Event::Closed ||
                    event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Escape) {
                    graphics.window.close();
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
