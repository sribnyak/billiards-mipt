#include "Table.h"

void Table::createBalls() {
    real yc = h / 2.0f;
    real xf = yc;
    real xb = w - xf;
    real dx = std::sqrt(3) * Ball::radius * 1.01f;
    real dy = Ball::radius * 1.01f;
    balls.emplace_back(Vector2(xf, yc), 0);
    for (int row = 0, id = 1; row < 5; ++row) {
        for (int place = 0; place < row + 1; ++place, ++id) {
            balls.emplace_back(Vector2(xb + real(row) * dx,
                                       yc + real(place * 2 - row) * dy),
                               id);
        }
    }
}

void Table::addVerticalBorder(real face, real top, real bottom) {
    borders.push_back(std::make_shared<VerticalBorder>(face, top, bottom));
}

void Table::addHorizontalBorder(real face, real left, real right) {
    borders.push_back(std::make_shared<HorizontalBorder>(face, left, right));
}

void Table::createBorders() {
    real center = w / 2.0f;
    real centerOffset = sizes::centralPocketWidth / 2.0f;
    real cornerOffset = sizes::cornerPocketWidth / std::sqrt(2);

    addVerticalBorder(0, cornerOffset, h - cornerOffset);
    addVerticalBorder(w, cornerOffset, h - cornerOffset);

    addHorizontalBorder(0, cornerOffset, center - centerOffset);
    addHorizontalBorder(0, center + centerOffset, w - cornerOffset);
    addHorizontalBorder(h, cornerOffset, center - centerOffset);
    addHorizontalBorder(h, center + centerOffset, w - cornerOffset);
}

void Table::processCollisions() {
    bool changed = true;
    int cnt = 30;
    while (changed && cnt) {
        --cnt;
        changed = false;
        for (int i = 0; i < balls.size(); ++i) {
            for (int j = i + 1; j < balls.size(); ++j) {
                changed |= balls[i].processCollision(balls[j]);
            }
            for (const auto& border : borders) {
                changed |= border->processCollision(balls[i]);
            }
        }
    }
}

real Table::timeWithoutCollisions(real maxTime) const {
    real time = maxTime;
    for (int i = 0; i < balls.size(); ++i) {
        for (int j = i + 1; j < balls.size(); ++j) {
            time = balls[i].timeUntilCollision(balls[j], time);
        }
        for (const auto& border : borders) {
            time = border->timeUntilCollision(balls[i], time);
        }
    }
    return time;
}

void Table::simulate(real time) {
    int cnt = 30;
    real timeLeft = time;
    while (timeLeft > 0 && cnt) {
        --cnt;
        // invariant: no objects are overlapping
        processCollisions();
        // invariant: all colliding objects won't overlap later

        real dt = timeWithoutCollisions(timeLeft); // dt > 0
        for (auto& ball : balls)
            ball.move(dt);
        for (auto it = balls.begin(); it != balls.end(); ++it) {
            if (it->position.x < 0 || it->position.x > w ||
                    it->position.y < 0 || it->position.y > h) {
                balls.erase(it);
                --it;
            }
        }

        timeLeft -= dt;
    }
    for (auto& ball : balls) {
        real v = length(ball.velocity);
        if (v > 0) {
            real dv = Table::frictionAcceleration * time;
            if (v - dv > 0) {
                ball.velocity -= dv * (ball.velocity / v);
            } else {
                ball.velocity = Vector2(0, 0);
            }
        }
    }
}

bool Table::ballsMoving() const {
    return std::any_of(balls.begin(), balls.end(), std::mem_fn(&Ball::moving));
}

Table::Table() {
    createBalls();
    createBorders();
}
