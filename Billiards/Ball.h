#ifndef BILLIARDS_BALL_H_
#define BILLIARDS_BALL_H_

#include "globals.h"
#include "maths.h"

class Ball {
public:
    static constexpr real radius = 0.05715;
    Vector2 position;
    Vector2 velocity;

    Ball(const Vector2& position, const Vector2& velocity = Vector2());
    void move(real t);
};


#endif //BILLIARDS_BALL_H_
