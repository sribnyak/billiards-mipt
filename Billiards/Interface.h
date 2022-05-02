#ifndef BILLIARDS_INTERFACE_H_
#define BILLIARDS_INTERFACE_H_

#include "globals.h"
#include "Ball.h"
#include "Border.h"
#include "Table.h"
#include <SFML/Graphics.hpp>

namespace settings {

extern real fps;
extern sf::Color tableColor;
extern sf::Color ballColor;
extern sf::Color borderColor;
extern sf::Color cueColor;
extern real scale;
extern const Vector2 origin;
Vector2 transform(const Vector2& vector);
extern unsigned int windowWidth;
extern unsigned int windowHeight;

} // namespace settings

class CueImage : public sf::Drawable {
    sf::ConvexShape shape;
public:
    bool onTable;
    CueImage();
    void setPosition(const Vector2& position);
    void setDirection(const Vector2& direction);
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

class TableImage : public sf::Drawable {
    sf::RectangleShape borders;
    sf::RectangleShape surface;
public:
    TableImage();
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

class Interface {
public:
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
    real time = 0;
    std::vector<sf::CircleShape> ballImages;
    CueImage cueImage;
    TableImage tableImage;

    void createBallImages(const std::vector<Ball>& balls);

    void demonstrate();
    void drawBall(const Ball& ball);

    void drawScene();
};

#endif // BILLIARDS_INTERFACE_H_
