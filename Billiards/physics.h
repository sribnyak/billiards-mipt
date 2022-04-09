#ifndef BILLIARDS_PHYSICS_H_
#define BILLIARDS_PHYSICS_H_

#include "globals.h"
#include "Ball.h"
#include "Table.h"
#include "Border.h"
#include "maths.h"

bool processCollision(Ball& ball1, Ball& ball2);
bool processCollision(Ball& ball, const Vector2& staticPoint);
bool processCollision(Ball& ball, const VerticalBorder& border);
bool processCollision(Ball& ball, const HorizontalBorder& border);

void processCollisions(Table& table);

real timeUntilCollision(const Ball&, const Ball&, real maxTime);
real timeUntilCollision(const Ball&, const VerticalBorder&, real maxTime);
real timeUntilCollision(const Ball&, const HorizontalBorder&, real maxTime);

real timeWithoutCollisions(const Table& table, real maxTime);

void simulate(Table& table, real time);

#endif // BILLIARDS_PHYSICS_H_
