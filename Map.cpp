#include "Map.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

// ─── Room ─────────────────────────────────
Room::Room(RoomType type)
    : type(type), enemy(nullptr), visited(false), locked(false) {}

RoomType Room::getType()    const { return type; }
bool     Room::hasEnemy()   const { return enemy && enemy->isAlive(); }
bool     Room::hasItems()   const { return !items.empty(); }
bool     Room::isVisited()  const { return visited; }
bool     Room::isLocked()   const { return locked; }
bool     Room::isPassable() const { return type != RoomType::WALL; }
Enemy*   Room::getEnemy()   const { return enemy; }
std::vector<Item>& Room::getItems() { return items; }

void Room::setEnemy(Enemy* e)   { enemy = e; }
void Room::addItem(Item item)   { items.push_back(std::move(item)); }
void Room::markVisited()        { visited = true; }
void Room::setLocked(bool l)    { locked = l; }
void Room::clearEnemy()         { enemy = nullptr; }

char Room::render(bool showFog) const {
    if (!visited && showFog) return ' ';
    switch (type) {
        case RoomType::WALL:       return '#';
        case RoomType::START:      return 'S';
        case RoomType::EXIT:       return locked ? 'X' : 'E';
        case RoomType::ITEM_ROOM:  return hasItems() ? (items.front().display()) : '.';
        case RoomType::ENEMY_ROOM: return hasEnemy() ? enemy->display() : '.';
        default:                   return visited ? '.' : ' ';
    }
}

// ─── Map ──────────────────────────────────
Map::Map(int w, int h) : width(w), height(h) {
    grid.resize(h, std::vector<Room>(w));
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

int Map::getWidth()  const { return width; }
int Map::getHeight() const { return height; }

Room& Map::getRoom(int x, int y) {
    return grid[y][x];
}
const Room& Map::getRoom(int x, int y) const {
    return grid[y][x];
}
bool Map::isValid(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

void Map::buildItemPool() {
    itemPool.emplace_back("Iron Sword",   ItemType::WEAPON,   8);
    itemPool.emplace_back("Health Potion",ItemType::POTION,  10);
    itemPool.emplace_back("Gold Coin",    ItemType::TREASURE, 5);
    itemPool.emplace_back("Old Key",      ItemType::KEY,      0);
    itemPool.emplace_back("Shield Shard", ItemType::WEAPON,   4);
    itemPool.emplace_back("Elixir",       ItemType::POTION,  20);
}

void Map::placeEnemies() {
    // Place 4 enemies at random non-border, non-start rooms
    for (int i = 0; i < 4; ++i) {
        int ex, ey;
        do {
            ex = 1 + std::rand() % (width - 2);
            ey = 1 + std::rand() % (height - 2);
        } while (grid[ey][ex].getType() != RoomType::EMPTY);

        bool aggressive = (i < 3);
        char sym = aggressive ? 'G' : 'T'; // Goblin / Troll
        std::string nm = aggressive ? "Goblin" : "Troll";
        int hp  = aggressive ? 10 : 18;
        int atk = aggressive ? 3  :  6;

        enemies.push_back(std::make_unique<Enemy>(nm, hp, atk, ex, ey, sym, aggressive));

        // Give some enemies a drop item
        if (!itemPool.empty()) {
            enemies.back()->setDrop(&itemPool[std::rand() % itemPool.size()]);
        }
        grid[ey][ex] = Room(RoomType::ENEMY_ROOM);
        grid[ey][ex].setEnemy(enemies.back().get());
    }
}

void Map::placeItems() {
    for (int i = 0; i < 3; ++i) {
        int ix, iy;
        do {
            ix = 1 + std::rand() % (width - 2);
            iy = 1 + std::rand() % (height - 2);
        } while (grid[iy][ix].getType() != RoomType::EMPTY);

        grid[iy][ix] = Room(RoomType::ITEM_ROOM);
        grid[iy][ix].addItem(itemPool[std::rand() % itemPool.size()]);
    }
}

void Map::generate() {
    // Border walls
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x) {
            if (x == 0 || x == width-1 || y == 0 || y == height-1)
                grid[y][x] = Room(RoomType::WALL);
            // Random interior walls (~15%)
            else if (std::rand() % 100 < 15)
                grid[y][x] = Room(RoomType::WALL);
            else
                grid[y][x] = Room(RoomType::EMPTY);
        }

    // Start top-left area, exit bottom-right area
    grid[1][1] = Room(RoomType::START);
    grid[1][1].markVisited();
    grid[height-2][width-2] = Room(RoomType::EXIT);
    grid[height-2][width-2].setLocked(true);

    buildItemPool();

    // Place key near center
    int kx = width / 2, ky = height / 2;
    grid[ky][kx] = Room(RoomType::ITEM_ROOM);
    grid[ky][kx].addItem(itemPool[3]); // Old Key

    placeItems();
    placeEnemies();
}

void Map::render(int px, int py, bool fogOfWar) const {
    const int sight = 3;
    std::cout << "\n";
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x == px && y == py) {
                std::cout << '@';
                continue;
            }
            bool inSight = (std::abs(x - px) <= sight && std::abs(y - py) <= sight);
            const Room& r = grid[y][x];
            if (fogOfWar && !r.isVisited() && !inSight)
                std::cout << ' ';
            else
                std::cout << r.render(false);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
