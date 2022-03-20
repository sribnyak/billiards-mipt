#ifndef BILLIARDS_PHYSICS_H
#define BILLIARDS_PHYSICS_H

#include "globals.h"
#include "objects.h"

real minQuadraticSolution(real a, real b, real c, real inf);

bool processCollision(Ball& ball1, Ball& ball2);
bool processCollision(Ball& ball, const Vec2r& staticPoint);
bool processCollision(Ball& ball, const VerticalBorder& border);
bool processCollision(Ball& ball, const HorizontalBorder& border);

void processCollisions();

real timeWithoutCollisions(real maxTime);

void simulate(real time);

#endif // BILLIARDS_PHYSICS_H
