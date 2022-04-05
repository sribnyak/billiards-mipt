#ifndef BILLIARDS_PHYSICS_H
#define BILLIARDS_PHYSICS_H

#include "globals.h"
#include "objects.h"
#include "maths.h"

real minQuadraticSolution(real a, real b, real c, real inf);

bool processCollision(Ball& ball1, Ball& ball2);
bool processCollision(Ball& ball, const Vec2r& staticPoint);
bool processCollision(Ball& ball, const VerticalBorder& border);
bool processCollision(Ball& ball, const HorizontalBorder& border);

void processCollisions();

real timeUntilCollision(const Ball&, const Ball&, real maxTime);
real timeUntilCollision(const Ball&, const VerticalBorder&, real maxTime);
real timeUntilCollision(const Ball&, const HorizontalBorder&, real maxTime);

real timeWithoutCollisions(real maxTime);

void simulate(real time);

#endif // BILLIARDS_PHYSICS_H
