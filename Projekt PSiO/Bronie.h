#pragma once
#include <string>
#include "Item.h"

class Bronie : public Item
{
protected:
    int zadawane_obrazenia;

public:
    Bronie(const std::string& d_nazwa, int d_zadawane_obrazenia, const std::string& d_typ)
        : Item(d_nazwa, d_typ), zadawane_obrazenia(d_zadawane_obrazenia)
    {
    }

    virtual ~Bronie() = default;

    std::string get_nazwa() const { return Item::nazwa; }
    int get_damage() const { return zadawane_obrazenia; }
    std::string get_typ() const { return Item::typ; }
};

// --- KONKRETNE BRONIE ---

class SlabyMiecz : public Bronie
{
public:
    SlabyMiecz()
        : Bronie("Slaby Miecz", 20, "Mele") {
    }
};

class ZwyklyMiecz : public Bronie
{
public:
    ZwyklyMiecz()
        : Bronie("Zwykly Miecz", 50, "Mele") {
    }
};

class DobryMiecz : public Bronie
{
public:
    DobryMiecz()
        : Bronie("Dobry Miecz", 100, "Mele") {
    }
};

class WysmienityMiecz : public Bronie
{
public:
    WysmienityMiecz()
        : Bronie("Wysmienity Miecz", 150, "Mele") {
    }
};

class KiepskiLuk : public Bronie
{
public:
    KiepskiLuk()
        : Bronie("Kiepski Luk", 20, "Range") {
    }
};

class ZwyklyLuk : public Bronie
{
public:
    ZwyklyLuk()
        : Bronie("Zwykly Luk", 40, "Range") {
    }
};

class WysmienityLuk : public Bronie
{
public:
    WysmienityLuk()
        : Bronie("Wysmienity Luk", 80, "Range") {
    }
};
