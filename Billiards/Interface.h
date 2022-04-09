#ifndef BILLIARDS_INTERFACE_H_
#define BILLIARDS_INTERFACE_H_

#include "globals.h"
#include "Ball.h"
#include "Border.h"
#include "Table.h"
#include <iostream>
#include <SFML/Graphics.hpp>

class Interface {
public:
    class Settings {
    public:
        real scale = 300;
        real fps = 30;

        Settings() = default;
        void change();
    };

    Settings settings;

    Interface(Table& table);
    ~Interface();
    void demonstrate(Table& table);
    Vector2 getStrikeVelocity();
    void showGameResult();
    void kill();
    bool isAlive();

private:
    sf::RenderWindow window;

    void drawBall(const Ball& ball);
    void drawBorder(const Border& border);

    void drawScene(const Table& table);
};

#endif // BILLIARDS_INTERFACE_H_
