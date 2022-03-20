#include "physics.h"

real minQuadraticSolution(real a, real b, real c, real inf) {
    real d = b * b - 4 * a * c;
    if (d < 0.00001) return inf; // TODO: eps needed?
    return - (b + std::sqrt(d)) / (2 * a);
}

bool processCollision(Ball& ball1, Ball& ball2) {
    Vec2r c = ball2.position - ball1.position;
    if (length(c) > 2 * Ball::radius) return false; // TODO: eps needed?
    Vec2r v = ball1.velocity;
    Vec2r u = ball2.velocity;
    Vec2r n = c / length(c);
    if (dotProduct(v - u, n) <= 0) return false; // TODO: eps needed?
    Vec2r vn = dotProduct(v, n) * n;
    Vec2r un = dotProduct(u, n) * n;
    ball1.velocity += un - vn;
    ball2.velocity += vn - un;
    return true;
}

bool processCollision(Ball& ball, const Vec2r& staticPoint) {
    Vec2r c = staticPoint - ball.position;
    if (length(c) > ball.radius) return false; // TODO: eps needed?
    Vec2r v = ball.velocity;
    if (dotProduct(v, c) <= 0) return false; // TODO: eps needed?
    Vec2r n = c / length(c);
    ball.velocity -= 2 * dotProduct(v, n) * n;
    return true;
}

bool processCollision(Ball& ball, const VerticalBorder& border) {
    if (std::abs(ball.position.x - border.face) > ball.radius ||
            ball.position.y < border.top() ||
            ball.position.y > border.bottom() ||
            ball.velocity.x == 0 ||
            ((ball.velocity.x > 0) != (border.face - ball.position.x) > 0)) {
        return (processCollision(ball, Vec2r(border.face, border.top())) ||
            processCollision(ball, Vec2r(border.face, border.bottom())));
    }
    ball.velocity.x = -ball.velocity.x;
    return true;
}

bool processCollision(Ball& ball, const HorizontalBorder& border) {
    if (std::abs(ball.position.y - border.face) > ball.radius ||
            ball.position.x < border.left() ||
            ball.position.x > border.right() ||
            ball.velocity.y == 0 ||
            ((ball.velocity.y > 0) != (border.face - ball.position.y) > 0)) {
        return (processCollision(ball, Vec2r(border.left(), border.face)) ||
            processCollision(ball, Vec2r(border.right(), border.face)));
    }
    ball.velocity.y = -ball.velocity.y;
    return true;
}

void processCollisions() {
    bool changed = true;
    int cnt = 30;
    while (changed && cnt) {
        --cnt;
        changed = false;
        for (int i = 0; i < balls.size(); ++i) if (!balls[i].pocketed) {
            for (int j = i + 1; j < balls.size(); ++j) if (!balls[j].pocketed)
                changed |= processCollision(balls[i], balls[j]);
            for (auto& border : verticalBorders)
                changed |= processCollision(balls[i], border);
            for (auto& border : horizontalBorders)
                changed |= processCollision(balls[i], border);
        }
    }
}

real timeWithoutCollisions(real maxTime) {
    return maxTime; // TODO: implement
}

void simulate(real time) {
    int cnt = 30;
    real timeLeft = time;
    while (timeLeft > 0 && cnt) { // TODO: eps needed?
        --cnt;

        processCollisions();

        real dt = timeWithoutCollisions(timeLeft);
        for (auto& ball : balls) if (!ball.pocketed)
            ball.move(dt);
        
        timeLeft -= dt;
    }
    for (auto& ball : balls) if (!ball.pocketed) {
        real v = length(ball.velocity);
        if (v > 0) {
            real dv = table::frictionAcceleration * time;
            if (v - dv > 0) {
                ball.velocity -= dv * (ball.velocity / v);
            } else {
                ball.velocity = Vec2r(0, 0);
            }
        }
    }
}
