#include "Entity.h"
#include <algorithm>
#include <sstream>
#include <iostream>

// ─── Entity ───────────────────────────────
Entity::Entity(const std::string& name, int hp, int atk, int x, int y)
    : name(name), hp(hp), maxHp(hp), atk(atk), x(x), y(y) {}

void Entity::takeDamage(int dmg) { hp = std::max(0, hp - dmg); }
bool Entity::isAlive()     const { return hp > 0; }
int  Entity::getHp()       const { return hp; }
int  Entity::getMaxHp()    const { return maxHp; }
int  Entity::getAtk()      const { return atk; }
int  Entity::getX()        const { return x; }
int  Entity::getY()        const { return y; }
const std::string& Entity::getName() const { return name; }
void Entity::setPosition(int nx, int ny) { x = nx; y = ny; }

// ─── Player ───────────────────────────────
Player::Player(int startX, int startY)
    : Entity("Hero", 30, 5, startX, startY), score(0), level(1), hasKey(false) {}

void Player::pickItem(Item item) {
    if (item.getType() == ItemType::KEY) hasKey = true;
    addScore(item.getValue());
    inventory.push_back(std::move(item));
    std::cout << "  Picked up: " << inventory.back().describe() << "\n";
}

void Player::usePotion() {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->getType() == ItemType::POTION) {
            int heal = it->getValue();
            hp = std::min(maxHp, hp + heal);
            std::cout << "  Used " << it->getName()
                      << " — restored " << heal << " HP.\n";
            inventory.erase(it);
            return;
        }
    }
    std::cout << "  No potions in inventory!\n";
}

bool Player::checkKey() const { return hasKey; }
int  Player::getScore()  const { return score; }
void Player::addScore(int s) { score += s; }

int  Player::attack() { return atk + level; }
char Player::display() const { return '@'; }

std::string Player::statusLine() const {
    std::ostringstream oss;
    oss << "[" << name << "] HP:" << hp << "/" << maxHp
        << " ATK:" << (atk + level)
        << " LVL:" << level
        << " Score:" << score
        << " Key:" << (hasKey ? "YES" : "no");
    return oss.str();
}

void Player::showInventory() const {
    if (inventory.empty()) { std::cout << "  (empty)\n"; return; }
    for (const auto& item : inventory)
        std::cout << "  " << item.describe() << "\n";
}

// ─── Enemy ───────────────────────────────
Enemy::Enemy(const std::string& name, int hp, int atk,
             int x, int y, char symbol, bool aggressive)
    : Entity(name, hp, atk, x, y), symbol(symbol),
      dropItem(nullptr), aggressive(aggressive) {}

Item*  Enemy::getDrop()        const { return dropItem; }
void   Enemy::setDrop(Item* item)    { dropItem = item; }
bool   Enemy::isAggressive()   const { return aggressive; }

void Enemy::aiMove(int px, int py) {
    if (!aggressive) return;
    // Move one step toward player
    if      (px > x) x++;
    else if (px < x) x--;
    if      (py > y) y++;
    else if (py < y) y--;
}

int  Enemy::attack()          { return atk; }
char Enemy::display()   const { return symbol; }

std::string Enemy::statusLine() const {
    std::ostringstream oss;
    oss << "[" << name << "] HP:" << hp << "/" << maxHp
        << " ATK:" << atk;
    return oss.str();
}
