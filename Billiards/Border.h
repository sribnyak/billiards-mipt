#ifndef BILLIARDS_BORDER_H_
#define BILLIARDS_BORDER_H_


#include "globals.h"
#include "maths.h"

class Border {
public:
    Vector2 topLeft;
    Vector2 bottomRight;

    Border(const Vector2& topLeft, const Vector2& bottomRight);
    virtual ~Border() = default;

    real left() const { return topLeft.x; }
    real right() const { return bottomRight.x; }
    real top() const { return topLeft.y; }
    real bottom() const { return bottomRight.y; }
};

class VerticalBorder : public Border {
public:
    using Border::Border;
    real face;
};

class HorizontalBorder : public Border {
public:
    using Border::Border;
    real face;
};

#endif //BILLIARDS_BORDER_H_
