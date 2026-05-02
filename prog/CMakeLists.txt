
include_directories(../lexGen)
include_directories(../syntaxGen)



find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

include(FetchContent)
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

BISON_TARGET(mParser parser.y ./parser.cpp DEFINES_FILE ./parser.hpp)
FLEX_TARGET(mLexer lexer.l ./lexer.cpp )

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(lab main.cpp ${BISON_mParser_OUTPUTS} ${FLEX_mLexer_OUTPUTS}
        game.cpp
        game.h
        arithNodes.h
        assignNodes.h
        baseNodes.h
        init.h
        diffValues.h
        castPrekols.h
        differentNodes.h
        robotActionNodes.h
        funtionNodes.h
        robot.h
        robot.cpp
        )

target_link_libraries(lab PRIVATE nlohmann_json::nlohmann_json)

#add_executable(lab main.cpp parser.cpp parser.hpp lex.yy.c)

#add_executable(lab2 main.cpp  parser2.tab.cc parser2.tab.hh lex.yy.cc)