#include "globals.h"

// time is in seconds
// distance is in meters
// everything is in SI

real scale = 300;
real fps = 30;

float dotProduct(const Vec2r& v, const Vec2r& u) {
    return v.x * u.x + v.y * u.y;
}
float dotSquare(const Vec2r& v) { return dotProduct(v, v); }
float length(const Vec2r& v) { return std::sqrt(dotSquare(v)); }
