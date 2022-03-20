#include "globals.h"
#include "objects.h"
#include "physics.h"

#include <iostream>

// void debug(sf::RenderWindow& window) {
//     // output
//     std::string s;
//     std::cin >> s;
//     if (s == "0") {
//         window.close();
//     }
// }

int main() {
    sf::RenderWindow window(
        sf::VideoMode(table::w * scale, table::h * scale),
        "Billiards"
    );

    GameState state = GameState::strike;

    createBalls();
    createBorders();

    sf::Clock clock;
    float time;
    window.setVerticalSyncEnabled(true);

    while (window.isOpen()) {
        window.clear();
        drawScene(window);
        window.display();
        
        if (state == GameState::simulation) {
            time = clock.getElapsedTime().asSeconds();
            if (time < 1 / fps) continue;
            time = 1 / fps;
            clock.restart();

            simulate(time);

            if (ballsStopped()) {
                state = (balls[0].pocketed || aliveBalls == 0
                    ? GameState::end : GameState::strike);
            }

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed ||
                        event.type == sf::Event::KeyPressed &&
                        event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        } else if (state == GameState::strike) {
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
