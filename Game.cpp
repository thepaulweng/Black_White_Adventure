#include "Game.h"
#include <iostream>
#include <limits>

Game::Game()
    : map(std::make_unique<Map>(16, 12)),
      player(std::make_unique<Player>(1, 1)),
      state(GameState::RUNNING),
      turnCount(0)
{
    map->generate();
}

void Game::printHelp() const {
    std::cout << "  Commands: w/a/s/d = move  i = inventory  p = use potion  h = help  q = quit\n";
}

void Game::printStatus() const {
    std::cout << "  Turn " << turnCount << "  " << player->statusLine() << "\n";
}

void Game::handleRoomEvent() {
    int px = player->getX(), py = player->getY();
    Room& room = map->getRoom(px, py);
    room.markVisited();

    // Enemy encounter
    if (room.hasEnemy()) {
        Enemy* enemy = room.getEnemy();
        std::cout << "\n  !! " << enemy->getName() << " appears! " << enemy->statusLine() << "\n";
        doCombat(*enemy);
    }

    // Pick up items if no enemy
    if (!room.hasEnemy() && room.hasItems()) {
        for (auto& item : room.getItems())
            player->pickItem(item);
        room.getItems().clear();
    }

    // Locked exit check
    Room& cur = map->getRoom(px, py);
    if (cur.getType() == RoomType::EXIT) {
        if (player->checkKey()) {
            std::cout << "\n  *** You unlocked the exit and escaped! ***\n";
            state = GameState::WIN;
        } else {
            std::cout << "\n  The exit is locked. You need a key!\n";
        }
    }
}

void Game::doCombat(Enemy& enemy) {
    while (enemy.isAlive() && player->isAlive()) {
        std::cout << "  [F]ight or [R]un? ";
        char c; std::cin >> c;
        if (c == 'r' || c == 'R') {
            std::cout << "  You flee!\n";
            return;
        }
        // Player attacks
        int dmg = player->attack();
        enemy.takeDamage(dmg);
        std::cout << "  You deal " << dmg << " damage. "
                  << enemy.getName() << " HP: " << enemy.getHp() << "\n";

        if (!enemy.isAlive()) {
            std::cout << "  " << enemy.getName() << " defeated!\n";
            player->addScore(20);
            // Drop item
            if (enemy.getDrop()) {
                player->pickItem(*enemy.getDrop());
            }
            map->getRoom(player->getX(), player->getY()).clearEnemy();
            return;
        }

        // Enemy attacks
        int edm = enemy.attack();
        player->takeDamage(edm);
        std::cout << "  " << enemy.getName() << " hits for " << edm
                  << " damage. Your HP: " << player->getHp() << "\n";
    }

    if (!player->isAlive()) {
        std::cout << "\n  You have been defeated...\n";
        state = GameState::LOSE;
    }
}

bool Game::processInput(char cmd) {
    int dx = 0, dy = 0;
    switch (cmd) {
        case 'w': dy = -1; break;
        case 's': dy =  1; break;
        case 'a': dx = -1; break;
        case 'd': dx =  1; break;
        case 'i':
            std::cout << "\n  Inventory:\n";
            player->showInventory();
            return true;
        case 'p':
            player->usePotion();
            return true;
        case 'h':
            printHelp();
            return true;
        case 'q':
            state = GameState::LOSE;
            std::cout << "  You quit the game.\n";
            return false;
        default:
            std::cout << "  Unknown command. Type 'h' for help.\n";
            return true;
    }

    int nx = player->getX() + dx;
    int ny = player->getY() + dy;

    if (!map->isValid(nx, ny) || !map->getRoom(nx, ny).isPassable()) {
        std::cout << "  Can't move there!\n";
        return true;
    }

    player->setPosition(nx, ny);
    turnCount++;
    handleRoomEvent();
    return true;
}

void Game::run() {
    std::cout << "============================\n";
    std::cout << "   Black & White Adventure  \n";
    std::cout << "============================\n";
    printHelp();

    while (state == GameState::RUNNING) {
        map->render(player->getX(), player->getY(), true);
        printStatus();

        std::cout << "  > ";
        char cmd;
        std::cin >> cmd;
        // Clear any leftover input
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        processInput(cmd);
    }

    if (state == GameState::WIN)
        std::cout << "\n  === YOU WIN! Final score: " << player->getScore() << " ===\n";
    else
        std::cout << "\n  === GAME OVER ===\n";
}
