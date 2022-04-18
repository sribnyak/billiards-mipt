#ifndef BILLIARDS_TABLE_H_
#define BILLIARDS_TABLE_H_

#include "globals.h"
#include "Ball.h"
#include "Border.h"
#include "maths.h"

class Table {
public:
    static constexpr real borderWidth = 0.05;
    static constexpr real h = 1.27 + 2 * borderWidth;
    static constexpr real w = 2.54 + 2 * borderWidth;
    static constexpr real frictionAcceleration = 0.07;

    std::vector<Ball> balls;
    std::vector<std::shared_ptr<Border>> borders;

    Table();
    void createBalls();
    std::shared_ptr<Border> createBorder(const Vector2&, const Vector2&);
    void createBorders();

    void processCollisions();
    real timeWithoutCollisions(real maxTime) const;
    void simulate(real time);
    bool ballsStopped() const;
    void drawScene();
};

#endif //BILLIARDS_TABLE_H_
