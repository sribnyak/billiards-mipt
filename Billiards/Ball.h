#ifndef BILLIARDS_BALL_H_
#define BILLIARDS_BALL_H_

#include "globals.h"
#include "maths.h"

class Ball {
public:
    static constexpr real radius = sizes::ballDiameter / 2.0f;
    Vector2 position;
    Vector2 velocity;
    int id;

    Ball(const Vector2& position, int id);
    void move(real t);
    bool processCollision(Ball& other);
    real timeUntilCollision(const Ball& other, real maxTime) const;
    bool moving() const;
};

#endif //BILLIARDS_BALL_H_
