#ifndef BILLIARDS_OBJECTS_H
#define BILLIARDS_OBJECTS_H

#include "globals.h"

namespace table {
    extern real h;
    extern real w;
    extern real frictionAcceleration;
}

class Ball{
public:
    static constexpr real radius = 0.05715;
    Vec2r position;
    Vec2r velocity;
    bool pocketed;
    sf::CircleShape image;

    Ball(const Vec2r& position, const Vec2r& velocity = Vec2r());
    void move(real t);
};

class Border{
public:
    Vec2r topLeft;
    Vec2r bottomRight;
    sf::RectangleShape image;

    Border(const Vec2r& topLeft, const Vec2r& bottomRight);
    real left() const { return topLeft.x; }
    real right() const { return bottomRight.x; }
    real top() const { return topLeft.y; }
    real bottom() const { return bottomRight.y; }
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

class Graphics {
public:
    sf::RenderWindow window;

    Graphics();

    template<typename T>
    void drawObject(T &obj);

    template<typename T>
    void drawObjects(vector<T> &objs);
};

class Settings {
public:
    real scale;
    real fps;

    Settings();
    void change();
};

class Table {
public:
    std::vector<Ball> balls;
    std::vector<VerticalBorder> verticalBorders;
    std::vector<HorizontalBorder> horizontalBorders;
    int aliveBalls;
    int pocketedBalls;
    real borderWidth;
    Graphics graphics;

    Table();
    void createBalls();
    void createBorders();
    bool ballsStopped();
    void drawScene();
};

class Game {
public:
    enum class GameState { strike, simulation, end };

    Table table;
    Settings settings;
    GameState state;

    Game();
    void mainLoop();
};

#endif // BILLIARDS_OBJECTS_H
