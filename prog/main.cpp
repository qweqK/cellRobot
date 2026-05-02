#include <iostream>
#include "../syntaxGen/help.h"
//#include "parser.h"
#include "parser.hpp"
#include "game.h"

int main() {
    std::cout << "Hello World!" << std::endl;
    Game game("empty.txt" , "map1.txt");
    game.run();
   // game.printTable();
}

// void yy::parser::error(const std::string &msg) {
//     std::cerr << msg << std::endl;
// }

