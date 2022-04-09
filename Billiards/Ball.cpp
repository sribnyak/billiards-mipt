#include "Ball.h"

Ball::Ball(const Vector2& position, const Vector2& velocity)
        : position(position), velocity(velocity)
{}

void Ball::move(real t) {
    position += t * velocity;
}
