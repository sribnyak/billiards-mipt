#include "Table.h"

void Table::createBalls() {
    real yc = h / 2.0f;
    real xf = yc;
    real xb = w - xf;
    real dx = std::sqrt(3) * Ball::radius * 1.01f;
    real dy = Ball::radius * 1.01f;
    balls.emplace_back(Vector2(xf, yc));
    balls.emplace_back(Vector2(xb, yc));
    balls.emplace_back(Vector2(xb + dx, yc - dy));
    balls.emplace_back(Vector2(xb + dx, yc + dy));
    balls.emplace_back(Vector2(xb + 2 * dx, yc - 2 * dy));
    balls.emplace_back(Vector2(xb + 2 * dx, yc));
    balls.emplace_back(Vector2(xb + 2 * dx, yc + 2 * dy));
}

std::shared_ptr<Border> Table::createBorder(const Vector2& topLeft,
                                            const Vector2& bottomRight) {
    Vector2 diagonal = bottomRight - topLeft;
    if (diagonal.x > diagonal.y) {
        auto ptr = std::make_shared<HorizontalBorder>(topLeft, bottomRight);
        ptr->face = (ptr->top() > h / 2 ? ptr->top() : ptr->bottom());
        return ptr;
    } else {
        auto ptr = std::make_shared<VerticalBorder>(topLeft, bottomRight);
        ptr->face = (ptr->left() > w / 2 ? ptr->left() : ptr->right());
        return ptr;
    }
}

void Table::createBorders() {
    real center = w / 2.f;
    real centerOffset = 1.4 * Ball::radius;
    real cornerOffset = borderWidth + 2 * Ball::radius;

    borders.emplace_back(createBorder(
            Vector2(0, cornerOffset),
            Vector2(borderWidth, h - cornerOffset)));
    borders.emplace_back(createBorder(
            Vector2(w - borderWidth, cornerOffset),
            Vector2(w, h - cornerOffset)));

    borders.emplace_back(createBorder(
            Vector2(cornerOffset, 0),
            Vector2(center - centerOffset, borderWidth)));
    borders.emplace_back(createBorder(
            Vector2(center + centerOffset, 0),
            Vector2(w - cornerOffset, borderWidth)));
    borders.emplace_back(createBorder(
            Vector2(cornerOffset, h - borderWidth),
            Vector2(center - centerOffset, h)));
    borders.emplace_back(createBorder(
            Vector2(center + centerOffset, h - borderWidth),
            Vector2(w - cornerOffset, h)));
}

bool Table::ballsStopped() {
    for (auto& ball : balls) {
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