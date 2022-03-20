#ifndef BILLIARDS_OBJECTS_H
#define BILLIARDS_OBJECTS_H

#include "globals.h"

extern real borderWidth;

namespace table {
    extern real h;
    extern real w;
    extern real frictionAcceleration;
}

class GameObject {
public:
    void draw(sf::RenderWindow& window) {} // virtual?
};

class Ball : public GameObject {
public:
    static constexpr real radius = 0.05715;
    Vec2r position;
    Vec2r velocity;
    bool pocketed;
    sf::CircleShape image;

    Ball(const Vec2r& position, const Vec2r& velocity = Vec2r());
    void move(real t);
    void draw(sf::RenderWindow& window);
};

class Border : public GameObject {
public:
    Border(const Vec2r& topLeft, const Vec2r& bottomRight);
    Vec2r topLeft;
    Vec2r bottomRight;
    sf::RectangleShape image;
    real left() const { return topLeft.x; }
    real right() const { return bottomRight.x; }
    real top() const { return topLeft.y; }
    real bottom() const { return bottomRight.y; }
    void draw(sf::RenderWindow& window);
};

class VerticalBorder : public Border {
public:
    VerticalBorder(const Vec2r& topLeft, const Vec2r& bottomRight);
    real face;
};

class HorizontalBorder : public Border {
public:
    HorizontalBorder(const Vec2r& topLeft, const Vec2r& bottomRight);
    real face;
};

extern std::vector<Ball> balls;
extern std::vector<VerticalBorder> verticalBorders;
extern std::vector<HorizontalBorder> horizontalBorders;

extern int aliveBalls;
extern int pocketedBalls;

void createBalls();
void createBorders();
bool ballsStopped();
void drawScene(sf::RenderWindow& window);

#endif // BILLIARDS_OBJECTS_H
