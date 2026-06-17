#pragma once
#include <string>
#include "Item.h"

class Pancerze : public Item
{
protected:
    int dodatkowy_def;

public:
    Pancerze(const std::string& d_nazwa, int d_dodatkowy_def, const std::string& d_typ)
        : Item(d_nazwa, d_typ), dodatkowy_def(d_dodatkowy_def)
    {
    }

    virtual ~Pancerze() = default;

    std::string get_nazwa() { return Item::nazwa; }
    int get_damage() { return dodatkowy_def; }
    std::string get_typ() { return Item::typ; }
};


class MizernyNapiersnik : public Pancerze
{
public:
    MizernyNapiersnik()
        : Pancerze("Mizerny Napiersnik", 30, "Klata") {
    }
};

class DobryNapiersnik : public Pancerze
{
public:
    DobryNapiersnik()
        : Pancerze("Dobry Napiersnik", 60, "Klata") {
    }
};

class KiepskiHelm : public Pancerze
{
public:
    KiepskiHelm()
        : Pancerze("Kiepski Helm", 10, "Helm") {
    }
};

class WybornyHelm : public Pancerze
{
public:
    WybornyHelm()
        : Pancerze("Wyborny Helm", 30, "Helm") {
    }
};

class BrudneSpodnie : public Pancerze
{
public:
    BrudneSpodnie()
        : Pancerze("Brudne Spodnie", 25, "Spodnie") {
    }
};

class PancerneSpodnie : public Pancerze
{
public:
    PancerneSpodnie()
        : Pancerze("Pancerne Spodnie", 50, "Spodnie") {
    }
};

class Trzewiki : public Pancerze
{
public:
    Trzewiki()
        : Pancerze("Trzewiki", 10, "Buty") {
    }
};

class WygodneOnuce : public Pancerze
{
public:
    WygodneOnuce()
        : Pancerze("Wygodne Onuce", 30, "Buty") {
    }
};
