#pragma once
#include <memory>
#include "Map.h"
#include "Entity.h"

enum class GameState {
    RUNNING,
    WIN,
    LOSE
};

class Game {
private:
    std::unique_ptr<Map>    map;
    std::unique_ptr<Player> player;
    GameState               state;
    int                     turnCount;

    void handleRoomEvent();
    void doCombat(Enemy& enemy);
    void printHelp() const;
    void printStatus() const;

public:
    Game();
    void run();

private:
    bool processInput(char cmd);
};
