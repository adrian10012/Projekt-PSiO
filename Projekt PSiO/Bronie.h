#pragma once
#include <string>
class Bronie
{
protected:
	std::string nazwa;
	int zadawane_obrazenia;
	std::string typ;

public:
	Bronie(const std::string& d_nazwa, int d_zadawane_obrazenia, std::string d_typ)
		: nazwa(d_nazwa), zadawane_obrazenia(d_zadawane_obrazenia), typ(d_typ) {}
	virtual ~Bronie() = default;

	std::string get_nazwa() { return nazwa; }
	int get_damage() { return zadawane_obrazenia; }
	std::string get_typ() { return typ; }
};

class SlabyMiecz : public Bronie
{
public:
	SlabyMiecz()
		: Bronie("Slaby Miecz", 20, "Mele") { }
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
		: Bronie("Kiepski Luk", 20, "Range") {}
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