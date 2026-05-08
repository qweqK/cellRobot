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
        size_t titlesize = 32;
        size_t heiht = gameMap.rows * titlesize;
        size_t width = gameMap.cols * titlesize;
        while (heiht > 1080 || width > 1920) {
            titlesize/=2;
            heiht = gameMap.rows * titlesize;
            width = gameMap.cols * titlesize;
        }
        std::cout << titlesize << std::endl;
        sf::RenderWindow window(sf::VideoMode(width, heiht), "TD");
        window.setPosition({0,0});
        window.setFramerateLimit(60);
        visualiser = std::make_unique<Visualiser>(titlesize, window);
        visualiser->loadTexture(pathGaphic);
        visualiser->loadSprite();
        visualiser->draw(data.screens);

    }
}