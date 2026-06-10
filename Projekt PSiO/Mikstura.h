#pragma once
#include <string>
#include "Item.h"

class Mikstura : public Item
{
protected:
    int wartosc;
    float czas;

public:
    Mikstura(const std::string& n, int w, float c)
        : Item(n, "Mikstura"), wartosc(w), czas(c)
    {
    }

    virtual ~Mikstura() = default;

    std::string getNazwa() const { return Item::nazwa; }
    int getWartosc() const { return wartosc; }
    float getCzas() const { return czas; }
};

// --- KONKRETNE MIKSTURY ---

class MalaMiksturaZdrowia : public Mikstura
{
public:
    MalaMiksturaZdrowia()
        : Mikstura("Ma³a mikstura zdrowia", 25, 0.f) {
    }
};

class MalaMiksturaRegeneracjiZdrowia : public Mikstura
{
public:
    MalaMiksturaRegeneracjiZdrowia()
        : Mikstura("Ma³a mikstura regeneracji zdrowia", 5, 10.f) {
    }
};

class MalaMiksturaSily : public Mikstura
{
public:
    MalaMiksturaSily()
        : Mikstura("Ma³a mikstura si³y", 10, 15.f) {
    }
};

class MalaMiksturaArmora : public Mikstura
{
public:
    MalaMiksturaArmora()
        : Mikstura("Ma³a mikstura pancerza", 15, 20.f) {
    }
};

class DuzaMiksturaZdrowia : public Mikstura
{
public:
    DuzaMiksturaZdrowia()
        : Mikstura("Du¿a mikstura zdrowia", 75, 0.f) {
    }
};

class DuzaMiksturaRegeneracjiZdrowia : public Mikstura
{
public:
    DuzaMiksturaRegeneracjiZdrowia()
        : Mikstura("Du¿a mikstura regeneracji zdrowia", 10, 15.f) {
    }
};

class DuzaMiksturaSily : public Mikstura
{
public:
    DuzaMiksturaSily()
        : Mikstura("Du¿a mikstura si³y", 25, 25.f) {
    }
};

class DuzaMiksturaArmora : public Mikstura
{
public:
    DuzaMiksturaArmora()
        : Mikstura("Du¿a mikstura pancerza", 35, 30.f) {
    }
};
