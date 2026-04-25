#include "Item.h"
#include <sstream>

Item::Item(const std::string& name, ItemType type, int value)
    : name(name), type(type), value(value) {}

const std::string& Item::getName()  const { return name; }
ItemType           Item::getType()  const { return type; }
int                Item::getValue() const { return value; }

char Item::display() const {
    switch (type) {
        case ItemType::WEAPON:   return 'W';
        case ItemType::POTION:   return 'P';
        case ItemType::KEY:      return 'K';
        case ItemType::TREASURE: return '$';
        default:                 return '?';
    }
}

std::string Item::describe() const {
    std::ostringstream oss;
    oss << "[" << display() << "] " << name << " (value: " << value << ")";
    return oss.str();
}
