#pragma once
#include <string>
class Pancerze
{
protected:
	std::string nazwa;
	int dodatkowy_def;
	std::string typ;

public:
	Pancerze(const std::string& d_nazwa, int d_dodatkowy_def, std::string d_typ)
		: nazwa(d_nazwa), dodatkowy_def(d_dodatkowy_def), typ(d_typ) {
	}
	virtual ~Pancerze() = default;

	std::string get_nazwa() { return nazwa; }
	int get_damage() { return dodatkowy_def; }
	std::string get_typ() { return typ; }
};

class MizernyNapiersnik : public Pancerze
{
public:
	MizernyNapiersnik()
		: Pancerze("Mizerny Napiersnik", 30, "Klata"){}
};

class DobryNapiersnik : public Pancerze
{
public:
	DobryNapiersnik()
		: Pancerze("Dobry Napiersnik", 60, "Klata"){}
};

class KiepskiHelm : public Pancerze
{
public:
	KiepskiHelm()
		: Pancerze("Kiepski Helm", 10, "Helm"){ }
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
		: Pancerze("Brudne Spodnie", 25, "Spodnie"){ }
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
		: Pancerze("Trzewiki", 10, "Buty"){ }
};

class WygodneOnuce : public Pancerze
{
public:
	WygodneOnuce()
		: Pancerze("Wygodne Onuce", 30, "Buty") { }
};