#include "maths.h"

Vector2::Vector2(const sf::Vector2<real>& vector) : sf::Vector2<real>(vector) {}

real Vector2::dot(const Vector2& other) const {
    return x * other.x + y * other.y;
}

real Vector2::squared() const { return this->dot(*this); }

real length(const Vector2& vector) { return std::sqrt(vector.squared()); }

real minQuadraticSolution(real a, real b, real c, real inf) {
    real d = b * b - 4 * a * c;
    if (d < 0) return inf; // domain error
    return - (b + std::sqrt(d)) / (2 * a);
}
