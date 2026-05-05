#include "visualiser.h"

#include <thread>
#include <variant>
void Visualiser::loadTexture(const std::string &path) {
    textures["0000"].loadFromFile(path + "0000.png");
    textures["0001"].loadFromFile(path + "0001.png");
    textures["0010"].loadFromFile(path + "0010.png");
    textures["0011"].loadFromFile(path + "0011.png");
    textures["0100"].loadFromFile(path + "0100.png");
    textures["0101"].loadFromFile(path + "0101.png");
    textures["0110"].loadFromFile(path + "0110.png");
    textures["0111"].loadFromFile(path + "0111.png");
    textures["1000"].loadFromFile(path + "1000.png");
    textures["1001"].loadFromFile(path + "1001.png");
    textures["1010"].loadFromFile(path + "1010.png");
    textures["1011"].loadFromFile(path + "1011.png");
    textures["1100"].loadFromFile(path + "1100.png");
    textures["1101"].loadFromFile(path + "1101.png");
    textures["1110"].loadFromFile(path + "1110.png");
    textures["1111"].loadFromFile(path + "1111.png");
    textures["robot"].loadFromFile(path + "robot.png");
    textures["exit"].loadFromFile(path + "exit.png");


}

void Visualiser::loadSprite() {
    sprites["0000"].setTexture(textures["0000"]);
    sprites["0000"].scale({titleSize/sprites["0000"].getGlobalBounds().width, titleSize/sprites["0000"].getGlobalBounds().height});

    sprites["0001"].setTexture(textures["0001"]);
    sprites["0001"].scale({titleSize/sprites["0001"].getGlobalBounds().width, titleSize/sprites["0001"].getGlobalBounds().height});

    sprites["0010"].setTexture(textures["0010"]);
    sprites["0010"].scale({titleSize/sprites["0010"].getGlobalBounds().width, titleSize/sprites["0010"].getGlobalBounds().height});

    sprites["0011"].setTexture(textures["0011"]);
    sprites["0011"].scale({titleSize/sprites["0011"].getGlobalBounds().width, titleSize/sprites["0011"].getGlobalBounds().height});

    sprites["0100"].setTexture(textures["0100"]);
    sprites["0100"].scale({titleSize/sprites["0100"].getGlobalBounds().width, titleSize/sprites["0100"].getGlobalBounds().height});

    sprites["0101"].setTexture(textures["0101"]);
    sprites["0101"].scale({titleSize/sprites["0101"].getGlobalBounds().width, titleSize/sprites["0101"].getGlobalBounds().height});

    sprites["0110"].setTexture(textures["0110"]);
    sprites["0110"].scale({titleSize/sprites["0110"].getGlobalBounds().width, titleSize/sprites["0110"].getGlobalBounds().height});

    sprites["0111"].setTexture(textures["0111"]);
    sprites["0111"].scale({titleSize/sprites["0111"].getGlobalBounds().width, titleSize/sprites["0111"].getGlobalBounds().height});

    sprites["1000"].setTexture(textures["1000"]);
    sprites["1000"].scale({titleSize/sprites["1000"].getGlobalBounds().width, titleSize/sprites["1000"].getGlobalBounds().height});

    sprites["1001"].setTexture(textures["1001"]);
    sprites["1001"].scale({titleSize/sprites["1001"].getGlobalBounds().width, titleSize/sprites["1001"].getGlobalBounds().height});

    sprites["1010"].setTexture(textures["1010"]);
    sprites["1010"].scale({titleSize/sprites["1010"].getGlobalBounds().width, titleSize/sprites["1010"].getGlobalBounds().height});

    sprites["1011"].setTexture(textures["1011"]);
    sprites["1011"].scale({titleSize/sprites["1011"].getGlobalBounds().width, titleSize/sprites["1011"].getGlobalBounds().height});

    sprites["1100"].setTexture(textures["1100"]);
    sprites["1100"].scale({titleSize/sprites["1100"].getGlobalBounds().width, titleSize/sprites["1100"].getGlobalBounds().height});

    sprites["1101"].setTexture(textures["1101"]);
    sprites["1101"].scale({titleSize/sprites["1101"].getGlobalBounds().width, titleSize/sprites["1101"].getGlobalBounds().height});

    sprites["1110"].setTexture(textures["1110"]);
    sprites["1110"].scale({titleSize/sprites["1110"].getGlobalBounds().width, titleSize/sprites["1110"].getGlobalBounds().height});

    sprites["1111"].setTexture(textures["1111"]);
    sprites["1111"].scale({titleSize/sprites["1111"].getGlobalBounds().width, titleSize/sprites["1111"].getGlobalBounds().height});

    sprites["robot"].setTexture(textures["robot"]);
    sprites["robot"].scale({titleSize/sprites["robot"].getGlobalBounds().width/1.5f, titleSize/sprites["robot"].getGlobalBounds().height/1.5f});
    sprites["robot"].setOrigin({sprites["robot"].getLocalBounds().width/2.f, sprites["robot"].getLocalBounds().height/2.f});

    sprites["exit"].setTexture(textures["exit"]);
    sprites["exit"].scale({titleSize/sprites["exit"].getGlobalBounds().width, titleSize/sprites["exit"].getGlobalBounds().height});


}


void Visualiser::draw(std::vector<GameMap> &screens) {

    for (auto &map : screens) {
        if (!window.isOpen()) { break;}
        window.clear();
        drawMap(map.grid, map.rows, map.cols);
        drawExit(map.exitX, map.exitY);
        drawRobot(map.rX,map.rY);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

}

void Visualiser::drawMap(Matrix &map, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            sf::Sprite s;
            std::string st = "";
            for (int k =0; k < 4; k++) {
                if (std::get<Cell>(map(i,j)).data[k]) st += '1';
                else st += '0';
            }
            s = sprites[st];
            s.setPosition(sf::Vector2f(j*titleSize, i * titleSize));
            window.draw(s);
        }
    }
}



void Visualiser::drawExit(int row, int col) {
    sf::Sprite s = sprites["exit"];
    s.setPosition(sf::Vector2f(col * titleSize, row * titleSize));
    window.draw(s);
}



void Visualiser::drawRobot(int x, int y) {
    sf::Sprite s = sprites["robot"];
    s.setPosition(sf::Vector2f(x * titleSize + titleSize/2.f, y * titleSize + titleSize/2.f));
    window.draw(s);
}
