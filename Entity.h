#pragma once
#include <string>
#include <vector>
#include "Item.h"

// ─────────────────────────────────────────
//  Abstract base class
// ─────────────────────────────────────────
class Entity {
protected:
    std::string name;
    int hp, maxHp;
    int atk;
    int x, y;

public:
    Entity(const std::string& name, int hp, int atk, int x, int y);
    virtual ~Entity() = default;

    void takeDamage(int dmg);
    bool isAlive()     const;
    int  getHp()       const;
    int  getMaxHp()    const;
    int  getAtk()      const;
    int  getX()        const;
    int  getY()        const;
    const std::string& getName() const;

    void setPosition(int nx, int ny);

    virtual int         attack()          = 0;   // returns damage dealt
    virtual char        display()   const = 0;
    virtual std::string statusLine() const = 0;
};

// ─────────────────────────────────────────
//  Player
// ─────────────────────────────────────────
class Player : public Entity {
private:
    std::vector<Item> inventory;
    int score;
    int level;
    bool hasKey;

public:
    Player(int startX, int startY);

    void pickItem(Item item);
    void usePotion();
    bool checkKey() const;
    int  getScore() const;
    void addScore(int s);

    int         attack()           override;
    char        display()    const override;
    std::string statusLine() const override;

    void showInventory() const;
};

// ─────────────────────────────────────────
//  Enemy
// ─────────────────────────────────────────
class Enemy : public Entity {
private:
    char   symbol;
    Item*  dropItem;   // nullable, owned externally
    bool   aggressive;

public:
    Enemy(const std::string& name, int hp, int atk,
          int x, int y, char symbol, bool aggressive = true);

    Item*       getDrop()   const;
    void        setDrop(Item* item);
    bool        isAggressive() const;

    // Simple AI: move toward player if aggressive
    void aiMove(int playerX, int playerY);

    int         attack()           override;
    char        display()    const override;
    std::string statusLine() const override;
};
