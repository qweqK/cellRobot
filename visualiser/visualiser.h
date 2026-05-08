#pragma once
#include <SFML/Graphics.hpp>
#include "../prog/robot.h"

class Visualiser {
    public:
    sf::RenderWindow &window;
    float titleSize;
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Sprite> sprites;
    Visualiser(float titleSize, sf::RenderWindow &window) : window(window), titleSize(titleSize), lastPosRob( {1000000, 1000000}) {};
    std::map<std::pair<int, int>, int> marked;
    std::pair<int, int> lastPosRob;
public:
    void loadTexture(const std::string & path);
    void loadSprite();
    void draw(std::vector<GameMap> &screens);
    void drawMap(Matrix &, int, int);
    void drawRobot(int, int);
    void drawExit(int, int);
};