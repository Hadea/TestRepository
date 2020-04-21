#pragma once
#include <iostream>
//#include "Color.h"

// NICHT Ändern!!
enum class SpielfeldEintrag : unsigned char
{
	Leer,
	WeissBauer,
	WeissTurm,
	WeissSpringer,
	WeissLaufer,
	WeissDame,
	WeissKoenig,
	WeissBauerGeist,
	SchwarzBauer,
	SchwarzTurm,
	SchwarzSpringer,
	SchwarzLaufer,
	SchwarzDame,
	SchwarzKoenig,
	SchwarzBauerGeist
};

enum class Spielzug : unsigned char
{
	Konform, Illegal, Schach, SchachMatt, Patt, FigurenTausch
};

enum class Color : uint8_t
{
	Green,
	Red,
	Blue,
	Yellow,
	BGWhite,
	BGBlack,
	Reset
};

enum class SaveGame
{
	FigurenTausch,
	SchachMatt
};

class Schachbrett final
{
public:

	bool SpielerWeissAmZug = true;
	//TODO: Killliste aller zerstörten Einheiten :D
	//TODO: Zeitanzeige (vorsicht mit Input vom Spieler)

	/*	Zahl Koordinate X, Buchstabe für Koordinate Y
		Prüft ob ein Zug regelkonform ist.
		Retuns: Konform, Illegal, Schach ,SchachMatt, Patt*/
	Spielzug Zug(std::string pQuelle, std::string pZiel);
	void EinheitenTausch(char pEinheitenName, std::string pZiel);
	void Draw();

	/*
		Setzt das Schachbrett auf die Anfangsposition
	*/
	void Reset();
#ifdef _DEBUG
	void Reset(const SaveGame pID);
#endif // DEBUG
	std::string ColorCode(Color pColor);
private:
	SpielfeldEintrag Spielfeld[8][8];
	uint16_t mZugZaehler=1;
	SpielfeldEintrag* GhostPointer = nullptr;
};

