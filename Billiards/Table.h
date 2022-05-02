#ifndef BILLIARDS_TABLE_H_
#define BILLIARDS_TABLE_H_

#include "globals.h"
#include "Ball.h"
#include "Border.h"
#include "maths.h"

class Table {
public:
    real h = sizes::tableHeight;
    real w = sizes::tableWidth;
    real frictionAcceleration = 0.95;

    std::vector<Ball> balls;
    std::vector<std::shared_ptr<Border>> borders;

    Table();
    void createBalls();
    void addVerticalBorder(real face, real top, real bottom);
    void addHorizontalBorder(real face, real left, real right);
    void createBorders();

    void processCollisions();
    real timeWithoutCollisions(real maxTime) const;
    void simulate(real time);
    bool ballsMoving() const;
};

#endif //BILLIARDS_TABLE_H_
