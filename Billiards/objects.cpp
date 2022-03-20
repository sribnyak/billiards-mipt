#include "objects.h"

real borderWidth = 0.05;

namespace table {
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
    if (position.x < -radius || position.x > table::w + radius ||
            position.y < -radius || position.y > table::h + radius) {
        pocketed = true;
        ++pocketedBalls;
        --aliveBalls;
        velocity = Vec2r(0, 0);
    }
}
void Ball::draw(sf::RenderWindow& window) {
    window.draw(image);
}

Border::Border(const Vec2r& topLeft, const Vec2r& bottomRight)
    : topLeft(topLeft), bottomRight(bottomRight) {
    Vec2r diag = bottomRight - topLeft;
    image.setSize(diag * scale);
    image.setPosition(topLeft * scale);
}
void Border::draw(sf::RenderWindow& window) {
    window.draw(image);
}

VerticalBorder::VerticalBorder(const Vec2r& topLeft, const Vec2r& bottomRight)
    : Border(topLeft, bottomRight) {
    face = (left() > table::w / 2 ? left() : right());
}
HorizontalBorder::HorizontalBorder(const Vec2r& topLeft, const Vec2r& bottomRight)
    : Border(topLeft, bottomRight) {
    face = (top() > table::h / 2 ? top() : bottom());
}

std::vector<Ball> balls;
std::vector<VerticalBorder> verticalBorders;
std::vector<HorizontalBorder> horizontalBorders;

int aliveBalls;
int pocketedBalls;

void createBalls() {
    real yc = table::h / 2.0f;
    real xf = yc;
    real xb = table::w - xf;
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

void createBorders() {
    real center = table::w / 2.f;
    real centerOffset = 1.4 * Ball::radius;
    real cornerOffset = borderWidth + 2 * Ball::radius;

    verticalBorders.emplace_back(Vec2r(0, cornerOffset),
        Vec2r(borderWidth, table::h - cornerOffset));
    verticalBorders.emplace_back(Vec2r(table::w - borderWidth, cornerOffset),
        Vec2r(table::w, table::h - cornerOffset));

    horizontalBorders.emplace_back(Vec2r(cornerOffset, 0),
        Vec2r(center - centerOffset, borderWidth));
    horizontalBorders.emplace_back(Vec2r(center + centerOffset, 0),
        Vec2r(table::w - cornerOffset, borderWidth));
    horizontalBorders.emplace_back(
        Vec2r(cornerOffset, table::h - borderWidth),
        Vec2r(center - centerOffset, table::h));
    horizontalBorders.emplace_back(
        Vec2r(center + centerOffset, table::h - borderWidth),
        Vec2r(table::w - cornerOffset, table::h));
}

bool ballsStopped() {
    for (auto& ball : balls) if (!ball.pocketed) {
        if (ball.velocity.x != 0 || ball.velocity.y != 0) {
            return false;
        }
    }
    return true;
}

void drawScene(sf::RenderWindow& window) {
    for (auto& ball : balls) if (!ball.pocketed) {
        ball.draw(window);
    }
    for (auto& border : verticalBorders) {
        border.draw(window);
    }
    for (auto& border : horizontalBorders) {
        border.draw(window);
    }
}
