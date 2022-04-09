#ifndef BILLIARDS_GAME_H_
#define BILLIARDS_GAME_H_

#include "Table.h"
#include "Interface.h"
#include "physics.h"

class Game {
public:
    enum class GameState { strike, simulation, end };

    Table table;
    GameState state;
    Interface interface;

    Game();
    void mainLoop();
};

#endif // BILLIARDS_GAME_H_
