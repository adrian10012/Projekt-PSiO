#pragma once
#include <string>

class Item
{
public:
    std::string nazwa;
    std::string typ;

    Item(const std::string& n, const std::string& t)
        : nazwa(n), typ(t) {
    }

    virtual ~Item() = default;
};
