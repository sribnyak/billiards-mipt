#include "Border.h"

Border::Border(const Vector2& topLeft, const Vector2& bottomRight)
        : topLeft(topLeft), bottomRight(bottomRight)
{}

bool Border::processEdgeCollision(const Vector2& edge, Ball& ball) const {
    Vector2 c = edge - ball.position;
    if (length(c) > ball.radius * 1.001) return false;
    Vector2 v = ball.velocity;
    if (v.dot(c) <= 0) return false;
    Vector2 n = c / length(c);
    ball.velocity -= 2 * v.dot(n) * n;
    return true;
}

bool VerticalBorder::processCollision(Ball& ball) const {
    if (std::abs(ball.position.x - face) > ball.radius * 1.001 ||
            ball.position.y < top() ||
            ball.position.y > bottom() ||
            ball.velocity.x == 0 ||
            ((ball.velocity.x > 0) != (face - ball.position.x) > 0)) {
        return (processEdgeCollision(Vector2(face, top()), ball) ||
                processEdgeCollision(Vector2(face, bottom()), ball));
    }
    ball.velocity.x = -ball.velocity.x;
    return true;
}

real VerticalBorder::timeUntilCollision(const Ball& ball, real maxTime) const {
    if (ball.velocity.x == 0) return maxTime;
    real t = std::min((face - ball.radius - ball.position.x) / ball.velocity.x,
                      (face + ball.radius - ball.position.x) / ball.velocity.x);
    if (t < 0.0001 || t > maxTime) return maxTime;
    real p = ball.position.y + ball.velocity.y * t;
    if (p < top() || p > bottom()) return maxTime;
    return t;
}

bool HorizontalBorder::processCollision(Ball& ball) const {
    if (std::abs(ball.position.y - face) > ball.radius * 1.001 ||
            ball.position.x < left() ||
            ball.position.x > right() ||
            ball.velocity.y == 0 ||
            ((ball.velocity.y > 0) != (face - ball.position.y) > 0)) {
        return (processEdgeCollision(Vector2(left(), face), ball) ||
                processEdgeCollision(Vector2(right(), face), ball));
    }
    ball.velocity.y = -ball.velocity.y;
    return true;
}

real HorizontalBorder::timeUntilCollision(const Ball& ball,
                                          real maxTime) const {
    if (ball.velocity.y == 0) return maxTime;
    real t = std::min((face - ball.radius - ball.position.y) / ball.velocity.y,
                      (face + ball.radius - ball.position.y) / ball.velocity.y);
    if (t < 0.0001 || t > maxTime) return maxTime;
    real p = ball.position.x + ball.velocity.x * t;
    if (p < left() || p > right()) return maxTime;
    return t;
}
