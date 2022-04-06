#include "physics.h"

bool processCollision(Ball& ball1, Ball& ball2) {
    Vector2 c = ball2.position - ball1.position;
    if (length(c) > 2 * Ball::radius * 1.001) return false;
    Vector2 v = ball1.velocity;
    Vector2 u = ball2.velocity;
    Vector2 n = c / length(c);
    if (n.dot(v - u) <= 0) return false;
    Vector2 vn = v.dot(n) * n;
    Vector2 un = u.dot(n) * n;
    ball1.velocity += un - vn;
    ball2.velocity += vn - un;
    return true;
}

bool processCollision(Ball& ball, const Vector2& staticPoint) {
    Vector2 c = staticPoint - ball.position;
    if (length(c) > ball.radius * 1.001) return false;
    Vector2 v = ball.velocity;
    if (v.dot(c) <= 0) return false;
    Vector2 n = c / length(c);
    ball.velocity -= 2 * v.dot(n) * n;
    return true;
}

bool processCollision(Ball& ball, const VerticalBorder& border) {
    if (std::abs(ball.position.x - border.face) > ball.radius * 1.001 ||
            ball.position.y < border.top() ||
            ball.position.y > border.bottom() ||
            ball.velocity.x == 0 ||
            ((ball.velocity.x > 0) != (border.face - ball.position.x) > 0)) {
        return (processCollision(ball, Vector2(border.face, border.top())) ||
            processCollision(ball, Vector2(border.face, border.bottom())));
    }
    ball.velocity.x = -ball.velocity.x;
    return true;
}

bool processCollision(Ball& ball, const HorizontalBorder& border) {
    if (std::abs(ball.position.y - border.face) > ball.radius * 1.001 ||
            ball.position.x < border.left() ||
            ball.position.x > border.right() ||
            ball.velocity.y == 0 ||
            ((ball.velocity.y > 0) != (border.face - ball.position.y) > 0)) {
        return (processCollision(ball, Vector2(border.left(), border.face)) ||
            processCollision(ball, Vector2(border.right(), border.face)));
    }
    ball.velocity.y = -ball.velocity.y;
    return true;
}

void processCollisions(Table& table) {
    bool changed = true;
    int cnt = 30;
    while (changed && cnt) {
        --cnt;
        changed = false;
        for (int i = 0; i < table.balls.size(); ++i) if (!table.balls[i].pocketed) {
            for (int j = i + 1; j < table.balls.size(); ++j) if (!table.balls[j].pocketed)
                changed |= processCollision(table.balls[i], table.balls[j]);
            for (auto& border : table.verticalBorders)
                changed |= processCollision(table.balls[i], border);
            for (auto& border : table.horizontalBorders)
                changed |= processCollision(table.balls[i], border);
        }
    }
}

real timeUntilCollision(const Ball& ball1, const Ball& ball2, real maxTime) {
    Vector2 r = ball1.position - ball2.position;
    Vector2 w = ball1.velocity - ball2.velocity;
    if (w.dot(r / length(r)) >= 0) return maxTime;
    real t = minQuadraticSolution(w.squared(), 2 * r.dot(w),
        r.squared() - 4 * Ball::radius * Ball::radius, maxTime);
    if (t < 0.0001 || t > maxTime) return maxTime;
    return t;
}

real timeUntilCollision(const Ball& ball, const VerticalBorder& border,
        real maxTime) {
    if (ball.velocity.x == 0) return maxTime;
    real t = std::min(
        (border.face - ball.radius - ball.position.x) / ball.velocity.x,
        (border.face + ball.radius - ball.position.x) / ball.velocity.x
    );
    if (t < 0.0001 || t > maxTime) return maxTime;
    real p = ball.position.y + ball.velocity.y * t;
    if (p < border.top() || p > border.bottom()) return maxTime;
    return t;
}

real timeUntilCollision(const Ball& ball, const HorizontalBorder& border,
        real maxTime) {
    if (ball.velocity.y == 0) return maxTime;
    real t = std::min(
        (border.face - ball.radius - ball.position.y) / ball.velocity.y,
        (border.face + ball.radius - ball.position.y) / ball.velocity.y
    );
    if (t < 0.0001 || t > maxTime) return maxTime;
    real p = ball.position.x + ball.velocity.x * t;
    if (p < border.left() || p > border.right()) return maxTime;
    return t;
}

real timeWithoutCollisions(const Table& table, real maxTime) {
    real time = maxTime;
    for (int i = 0; i < table.balls.size(); ++i) if (!table.balls[i].pocketed) {
        for (int j = i + 1; j < table.balls.size(); ++j) if (!table.balls[j].pocketed)
            time = timeUntilCollision(table.balls[i], table.balls[j], time);
        for (auto& border : table.verticalBorders)
            time = timeUntilCollision(table.balls[i], border, time);
        for (auto& border : table.horizontalBorders)
            time = timeUntilCollision(table.balls[i], border, time);
    }
    return time;
}

void simulate(Table& table, real time) {
    int cnt = 30;
    real timeLeft = time;
    while (timeLeft > 0 && cnt) { // TODO: eps needed?
        --cnt;
        // invariant: no objects are overlapping
        processCollisions(table);
        // invariant: all colliding objects won't overlap later

        real dt = timeWithoutCollisions(table, timeLeft); // dt > 0
        for (auto& ball : table.balls) if (!ball.pocketed)
            ball.move(dt);
        
        timeLeft -= dt;
    }
    for (auto& ball : table.balls) if (!ball.pocketed) {
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
