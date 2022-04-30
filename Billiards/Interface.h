#ifndef BILLIARDS_INTERFACE_H_
#define BILLIARDS_INTERFACE_H_

#include "globals.h"
#include "Ball.h"
#include "Border.h"
#include "Table.h"
#include <SFML/Graphics.hpp>

class CueImage {
    sf::ConvexShape shape;
public:
    bool onTable;
    CueImage(real scale, sf::Color color);
    void setPosition(const Vector2& position);
    void setDirection(const Vector2& direction);
    void draw(sf::RenderWindow& window);
};

class Interface {
public:
    class Settings {
    public:
        real scale = 300;
        real fps = 30;
        sf::Color tableColor{0x00, 0x88, 0x3e};
        sf::Color ballColor{0x74, 0x00, 0x14};
        sf::Color borderColor{0x55, 0x2c, 0x0e};
        sf::Color cueColor{0xbe, 0xb0, 0x62};

        Settings() = default;
        void change();
    };

    Settings settings;

    explicit Interface(Table& table);
    ~Interface();
    void simulate();
    Vector2 getStrikeVelocity();
    void showGameResult();
    void kill();
    bool isAlive();

private:
    sf::RenderWindow window;
    Table& table;
    sf::Clock clock;
    real time;
    std::vector<sf::CircleShape> ballImages;
    CueImage cueImage;

    void createBallImages(const std::vector<Ball>& balls);

    void demonstrate();
    void drawBall(const Ball& ball);
    void drawBorder(const Border& border);

    void drawScene();
};

#endif // BILLIARDS_INTERFACE_H_
