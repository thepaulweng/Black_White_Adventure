#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Entity.h"
#include "Item.h"

enum class RoomType {
    EMPTY,
    WALL,
    START,
    EXIT,
    ITEM_ROOM,
    ENEMY_ROOM
};

// ─────────────────────────────────────────
//  Room
// ─────────────────────────────────────────
class Room {
private:
    RoomType             type;
    std::vector<Item>    items;
    Enemy*               enemy;     // non-owning pointer
    bool                 visited;
    bool                 locked;    // exit locked by key

public:
    explicit Room(RoomType type = RoomType::EMPTY);

    RoomType getType()    const;
    bool     hasEnemy()   const;
    bool     hasItems()   const;
    bool     isVisited()  const;
    bool     isLocked()   const;
    bool     isPassable() const;

    Enemy*   getEnemy()   const;
    std::vector<Item>& getItems();

    void setEnemy(Enemy* e);
    void addItem(Item item);
    void markVisited();
    void setLocked(bool locked);
    void clearEnemy();

    // Returns the display char for the map
    char render(bool showFog) const;
};

// ─────────────────────────────────────────
//  Map
// ─────────────────────────────────────────
class Map {
private:
    int width, height;
    std::vector<std::vector<Room>>   grid;
    std::vector<std::unique_ptr<Enemy>> enemies;    // owns all enemies
    std::vector<Item>                itemPool;       // pre-built items

    void buildItemPool();
    void placeEnemies();
    void placeItems();

public:
    Map(int width, int height);

    int getWidth()  const;
    int getHeight() const;

    Room&       getRoom(int x, int y);
    const Room& getRoom(int x, int y) const;
    bool        isValid(int x, int y) const;

    void generate();
    void render(int playerX, int playerY, bool fogOfWar = true) const;
};
