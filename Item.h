#pragma once
#include <string>

enum class ItemType {
    WEAPON,
    POTION,
    KEY,
    TREASURE
};

class Item {
private:
    std::string name;
    ItemType    type;
    int         value;

public:
    Item(const std::string& name, ItemType type, int value);

    const std::string& getName()  const;
    ItemType           getType()  const;
    int                getValue() const;
    char               display()  const;

    // Returns description string for UI
    std::string describe() const;
};
