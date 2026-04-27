#include "game.h"

extern FILE* yyin;
void Game::run() {
   yyin = fopen(filename.c_str(), "r");
   parser.parse();
   fclose(yyin);
}

void Game::printTable() {
   for (auto & f : data.functions) {
      std::cout << "function: "<<f.first << "{" <<std::endl;
      for (auto & q : *f.second.varst) {
         std::cout << q.first << ": ";
         switch (q.second.value.type) {
            case VarType::SIGNED: std::cout << std::get<int>(q.second.value.s) << " -SIGNED";  break;
            case VarType::UNSIGNED: std::cout << std::get<unsigned int>(q.second.value.s) << " -UNSIGNED"; break;
            case VarType::CELL: std::cout <<std::endl << std::get<Cell>(q.second.value.s) << std::endl << " -CELL"; break;

         }
         if (q.second.isConst) { std::cout << " |const" << std::endl; }
         else {std::cout << " |not const" << std::endl; }
      }
      std::cout << "}" << std::endl;
   }
}

