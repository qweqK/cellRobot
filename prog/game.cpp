#include "game.h"

extern FILE* yyin;
void Game::run() {
    if (isGraphic == false) {
        yyin = fopen(filename.c_str(), "r");
        try {
            parser.parse();
        }
        catch (std::exception& e) {
            std::cerr << "catch error: " << e.what() << std::endl;
        }
    }
    else {
        yyin = fopen(filename.c_str(), "r");
        try {
            parser.parse();
        }
        catch (std::exception& e) {
            std::cerr << "catch error: " << e.what() << std::endl;
        }
        sf::RenderWindow window(sf::VideoMode(960, 640), "TD");
        window.setPosition({0,0});
        window.setFramerateLimit(60);
        visualiser = std::make_unique<Visualiser>(64.0f, window);
        visualiser->loadTexture(pathGaphic);
        visualiser->loadSprite();
        visualiser->draw(data.screens);

    }
}