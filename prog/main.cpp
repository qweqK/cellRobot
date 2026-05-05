#include <iostream>
#include "../syntaxGen/help.h"
//#include "parser.h"
#include "parser.hpp"
#include "game.h"

int main() {
    std::cout << "Hello World!" << std::endl;
    //Game game("testikus", "map1.txt");
    //Game game("testikus", "map1.txt", "graphic/");
    //Game game("error", "map1.txt");
    //Game game("test", "map1.txt");
   // Game game("empty", "map1.txt");
    //Game game("testikus", "map1.txt");
    //Game game("testikus", "map1.txt");
   // Game game("opersharp", "map1.txt");
    //Game game("matrarith", "map1.txt");
   // Game game("zerodiv", "map1.txt");
    // Game game("testXray", "map1.txt");
   Game game("labirintGame", "map1.txt", "graphic/");
   //Game game("empty.txt", "map1.txt");
   // Game game("cellTest", "map1.txt");
  //Game game("testGlob", "map1.txt");
 //Game game("yaustal", "map1.txt");



    game.run();

   // game.printTable();
}

// void yy::parser::error(const std::string &msg) {
//     std::cerr << msg << std::endl;
// }

