#ifndef BILLIARDS_OBJECTS_H_
#define BILLIARDS_OBJECTS_H_

#include "globals.h"
#include "maths.h"

extern int aliveBalls;
extern int pocketedBalls;

class Ball{
public:
    static constexpr real radius = 0.05715;
    Vector2 position;
    Vector2 velocity;
    bool pocketed;
    sf::CircleShape image;

    Ball(const Vector2& position, const Vector2& velocity = Vector2());
    void move(real t);
};

class Border{
public:
    Vector2 topLeft;
    Vector2 bottomRight;
    sf::RectangleShape image;

    Border(const Vector2& topLeft, const Vector2& bottomRight);
    real left() const { return topLeft.x; }
    real right() const { return bottomRight.x; }
    real top() const { return topLeft.y; }
    real bottom() const { return bottomRight.y; }
};

class VerticalBorder : public Border {
public:
    VerticalBorder(const Vector2& topLeft, const Vector2& bottomRight);
    real face;
};

class HorizontalBorder : public Border {
public:
    HorizontalBorder(const Vector2& topLeft, const Vector2& bottomRight);
    real face;
};

class Table {
public:
    static constexpr real borderWidth = 0.05;
    static constexpr real h = 1.27 + 2 * borderWidth;
    static constexpr real w = 2.54 + 2 * borderWidth;
    static constexpr real frictionAcceleration = 0.07;

    std::vector<Ball> balls;
    std::vector<VerticalBorder> verticalBorders;
    std::vector<HorizontalBorder> horizontalBorders;

    Table();
    void createBalls();
    void createBorders();
    bool ballsStopped();
    void drawScene();
};

class Settings {
public:
    static constexpr real scale = 300;
    real fps;

    Settings();
    void change();
};

#endif // BILLIARDS_OBJECTS_H_
