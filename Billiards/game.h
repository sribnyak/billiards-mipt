#ifndef BILLIARDS_GAME_H_
#define BILLIARDS_GAME_H_

#include "objects.h"
#include "interface.h"
#include "physics.h"

class Game {
public:
    enum class GameState { strike, simulation, end };

    Table table;
    Settings settings;
    GameState state;
    Interface interface;

    Game();
    void mainLoop();
};

#endif // BILLIARDS_GAME_H_
