#include "Game.h"

Game::Game() : table(), state(GameState::strike), interface(table) {}

void Game::mainLoop() {
    while (interface.isAlive()) {
        if (state == GameState::simulation) {
            interface.simulate();
            state = ((table.balls.empty() || table.balls[0].id != 0)
                     ? GameState::end : GameState::strike);
        } else if (state == GameState::strike) {
            table.balls[0].velocity = interface.getStrikeVelocity();
            state = GameState::simulation;
        } else if (state == GameState::end) {
            if (interface.restartAtTheEnd()) {
                table = Table();
                state = GameState::strike;
                interface.restart();
            } else {
                break;
            }
        }
    }
}
