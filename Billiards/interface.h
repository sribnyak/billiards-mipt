#ifndef BILLIARDS_INTERFACE_H_
#define BILLIARDS_INTERFACE_H_

#include "globals.h"
#include "objects.h"
#include <iostream>
#include <SFML/Graphics.hpp>

class Interface {
private:
    sf::RenderWindow window;

    template<typename T>
    void drawObject(T& obj);

    void drawScene(const Table& table);
public:
    Interface();
    ~Interface();
    void demonstrate(Table& table);
    Vector2 getStrikeVelocity();
    void showGameResult();
    void kill();
    bool isAlive();
};

#endif // BILLIARDS_INTERFACE_H_
