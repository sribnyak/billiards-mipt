#ifndef BILLIARDS_BORDER_H_
#define BILLIARDS_BORDER_H_

#include "globals.h"
#include "maths.h"
#include "Ball.h"

class Border {
public:
    static bool processEdgeCollision(const Vector2& edge, Ball& ball);

    virtual bool processCollision(Ball& ball) const = 0;
    virtual real timeUntilCollision(const Ball& ball, real maxTime) const = 0;
};

class VerticalBorder : public Border {
public:
    real face;
    real top;
    real bottom;
    VerticalBorder(real face, real top, real bottom)
            : face(face), top(top), bottom(bottom) {}
    bool processCollision(Ball& ball) const override;
    real timeUntilCollision(const Ball& ball, real maxTime) const override;
};

class HorizontalBorder : public Border {
public:
    real face;
    real left;
    real right;
    HorizontalBorder(real face, real left, real right)
            : face(face), left(left), right(right) {}
    bool processCollision(Ball& ball) const override;
    real timeUntilCollision(const Ball& ball, real maxTime) const override;
};

#endif //BILLIARDS_BORDER_H_
