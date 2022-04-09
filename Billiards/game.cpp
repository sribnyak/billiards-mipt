#include "game.h"

Game::Game()
    : table(), state(GameState::strike), interface(table)
{}

void Game::mainLoop() {
    sf::Clock clock;
    real time;
    while (interface.isAlive()) {
        interface.demonstrate(table);

        if (state == GameState::simulation) {
            time = clock.getElapsedTime().asSeconds();
            if (time < 1 / interface.settings.fps) continue;
            time = 1 / interface.settings.fps;
            clock.restart();

            simulate(table, time);

            if (table.ballsStopped()) {
                state = (table.balls.empty() ? GameState::end
                                             : GameState::strike);
            }
        } else if (state == GameState::strike) {
            table.balls[0].velocity = interface.getStrikeVelocity();
            state = GameState::simulation;
            clock.restart();
        } else if (state == GameState::end) {
            interface.showGameResult();
            break;
        }
    }
}
