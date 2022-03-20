#ifndef BILLIARDS_GLOBALS_H
#define BILLIARDS_GLOBALS_H

#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

using real = float;
using Vec2r = sf::Vector2<real>;

extern real scale;
extern real fps;

enum class GameState { strike, simulation, end };

float dotProduct(const Vec2r& v, const Vec2r& u);
float dotSquare(const Vec2r& v);
float length(const Vec2r& v);

#endif // BILLIARDS_GLOBALS_H
