#include "Ball.h"

Ball::Ball(const Vector2& position, int id)
        : position(position), velocity(Vector2(0, 0)), id(id)
{}

void Ball::move(real t) {
    position += t * velocity;
}

bool Ball::processCollision(Ball& other) {
    Vector2 c = other.position - position;
    if (length(c) > 2 * Ball::radius * 1.001) return false;
    Vector2 v = velocity;
    Vector2 u = other.velocity;
    Vector2 n = c / length(c);
    if (n.dot(v - u) <= 0) return false;
    Vector2 vn = v.dot(n) * n;
    Vector2 un = u.dot(n) * n;
    velocity += un - vn;
    other.velocity += vn - un;
    return true;
}

real Ball::timeUntilCollision(const Ball& other, real maxTime) const {
    Vector2 r = position - other.position;
    Vector2 w = velocity - other.velocity;
    if (w.dot(r / length(r)) >= 0) return maxTime;
    real t = minQuadraticSolution(w.squared(), 2 * r.dot(w),
        r.squared() - 4 * radius * radius, maxTime);
    if (t < 0.0001 || t > maxTime) return maxTime;
    return t;
}
