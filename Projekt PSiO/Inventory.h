#pragma once
#include <vector>
#include <memory>
#include "Mikstura.h"
#include "Pancerze.h"
#include "Bronie.h"
#include "Item.h"

class Inventory
{
public:
    std::vector<std::unique_ptr<Mikstura>> mikstury;

    std::unique_ptr<Pancerze> helm;
    std::unique_ptr<Pancerze> klata;
    std::unique_ptr<Pancerze> spodnie;
    std::unique_ptr<Pancerze> buty;

    std::unique_ptr<Bronie> bron;

    std::unique_ptr<Item> sloty[6];

    void addMikstura(std::unique_ptr<Mikstura> m)
    {
        mikstury.push_back(std::move(m));
    }

    void equipArmor(std::unique_ptr<Pancerze> p)
    {
        if (p->get_typ() == "Helm")      helm = std::move(p);
        else if (p->get_typ() == "Klata")   klata = std::move(p);
        else if (p->get_typ() == "Spodnie") spodnie = std::move(p);
        else if (p->get_typ() == "Buty")    buty = std::move(p);
    }

    void equipWeapon(std::unique_ptr<Bronie> b)
    {
        bron = std::move(b);
    }

    Mikstura* getMikstura(int index)
    {
        if (index < 0 || index >= (int)mikstury.size()) return nullptr;
        return mikstury[index].get();
    }

    void removeMikstura(int index)
    {
        if (index < 0 || index >= (int)mikstury.size()) return;
        mikstury.erase(mikstury.begin() + index);
    }

    bool addToFreeSlot(std::unique_ptr<Item> it)
    {
        for (int i = 0; i < 6; i++)
        {
            if (!sloty[i])
            {
                sloty[i] = std::move(it);
                return true;
            }
        }
        return false;
    }

    int findFreeSlot() const
    {
        for (int i = 0; i < 6; i++)
        {
            if (!sloty[i])
                return i;
        }
        return -1;
    }
};
