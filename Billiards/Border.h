#ifndef BILLIARDS_BORDER_H_
#define BILLIARDS_BORDER_H_

#include "globals.h"
#include "maths.h"
#include "Ball.h"

class Border {
public:
    Vector2 topLeft;
    Vector2 bottomRight;

    Border(const Vector2& topLeft, const Vector2& bottomRight);
    static bool processEdgeCollision(const Vector2& edge, Ball& ball);
    virtual bool processCollision(Ball& ball) const = 0;
    virtual real timeUntilCollision(const Ball& ball, real maxTime) const = 0;

    real left() const { return topLeft.x; }
    real right() const { return bottomRight.x; }
    real top() const { return topLeft.y; }
    real bottom() const { return bottomRight.y; }
};

class VerticalBorder : public Border {
public:
    using Border::Border;
    real face;
    bool processCollision(Ball& ball) const override;
    real timeUntilCollision(const Ball& ball, real maxTime) const override;
};

class HorizontalBorder : public Border {
public:
    using Border::Border;
    real face;
    bool processCollision(Ball& ball) const override;
    real timeUntilCollision(const Ball& ball, real maxTime) const override;
};

#endif //BILLIARDS_BORDER_H_
