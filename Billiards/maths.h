#ifndef BILLIARDS_MATHS_H_
#define BILLIARDS_MATHS_H_

#include "globals.h"
#include <SFML/Graphics.hpp>

class Vector2 : public sf::Vector2<real> {
public:
    using sf::Vector2<real>::Vector2;
    Vector2(const sf::Vector2<real>&);
    real dot(const Vector2& other) const;
    real squared() const;
};

real length(const Vector2& vector);

real minQuadraticSolution(real a, real b, real c, real inf);

#endif // BILLIARDS_MATHS_H_
