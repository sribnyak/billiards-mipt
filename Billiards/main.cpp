#include "globals.h"
#include "objects.h"
#include "physics.h"

#include <chrono>
#include <thread>
#include <iostream>

void sleep(real time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(int(time * 1000)));
}

int main() {
    GameState state = GameState::strike;

    createBalls();
    createBorders();

    sf::Clock clock;
    float time;

    while (true) {
        if (state == GameState::simulation) {
            time = clock.getElapsedTime().asSeconds();
//            sleep(std::max(int(1 / fps - time), 0));
            time = 1 / fps;
            clock.restart();

            simulate(time);

            if (ballsStopped()) {
                state = (balls[0].pocketed || aliveBalls == 0
                    ? GameState::end : GameState::strike);
            }
        } else if (state == GameState::strike) {
            std::cout << "Balls positions:\n";
            for (auto& ball : balls) {
                std::cout << ball.position.x << ' ' << ball.position.y;
                if (ball.pocketed) std::cout << " (pocketed)";
                std::cout << '\n';
            }
            std::cout << "Pocketed balls: " << pocketedBalls << '\n';
            std::cout << "Balls on the table: " << aliveBalls << '\n';
            real x, y;
            std::cout << "Give a velocity to the ball\n";
            std::cout << "x: " << std::flush;
            std::cin >> x;
            std::cout << "y: " << std::flush;
            std::cin >> y;
            balls[0].velocity.x = x;
            balls[0].velocity.y = y;
            state = GameState::simulation;
            clock.restart();
        } else if (state == GameState::end) {
            std::cout << "Game over" << std::endl;
            break;
        }
    }
    return 0;
}
