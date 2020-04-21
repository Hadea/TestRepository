#include "Schachbrett.h"

Spielzug Schachbrett::Zug(std::string pQuelle, std::string pZiel)
{
#pragma region InputTesting

	//TODO: Konvertierung von der Main in einer Funktion abhandeln lassen!

	uint8_t QuelleX;

	if (pQuelle[0] < 97)
		QuelleX = pQuelle[0] - 65;
	else
		QuelleX = pQuelle[0] - 97;


	uint8_t QuelleY = pQuelle[1] - 49;
	uint8_t ZielX;

	if (pZiel[0] < 97)
		ZielX = pZiel[0] - 65;
	else
		ZielX = pZiel[0] - 97;

	uint8_t ZielY = pZiel[1] - 49;

	if (QuelleX > 7 ||
		QuelleY > 7 ||
		ZielX > 7 ||
		ZielY > 7 ||
		(QuelleX == ZielX && QuelleY == ZielY) ||
		Spielfeld[QuelleY][QuelleX] == SpielfeldEintrag::Leer ||
		Spielfeld[QuelleY][QuelleX] == SpielfeldEintrag::WeissBauerGeist ||
		Spielfeld[QuelleY][QuelleX] == SpielfeldEintrag::SchwarzBauerGeist)
	{
		return Spielzug::Illegal;
	}

	if ((SpielerWeissAmZug && (int)(Spielfeld[QuelleY][QuelleX]) > 7) ||
		(!SpielerWeissAmZug && (int)(Spielfeld[QuelleY][QuelleX]) < 8) ||
		(!SpielerWeissAmZug && (int)(Spielfeld[ZielY][ZielX]) > 7) ||
		(SpielerWeissAmZug && (int)(Spielfeld[ZielY][ZielX]) < 8 && (Spielfeld[ZielY][ZielX]) != SpielfeldEintrag::Leer) // auf das leere feld 0 achten
		)
	{
		return Spielzug::Illegal;
	}

#pragma endregion
	// eingaben verarbeiten
	//HACK: zu früh getestet, zug könnte illegal sein
	if (GhostPointer != nullptr)
	{
		if (SpielerWeissAmZug && Spielfeld[ZielY][ZielX] != SpielfeldEintrag::SchwarzBauerGeist)
		{
			*GhostPointer = SpielfeldEintrag::Leer;
			GhostPointer = nullptr;
		}

		if (!SpielerWeissAmZug && Spielfeld[ZielY][ZielX] != SpielfeldEintrag::WeissBauerGeist)
		{
			*GhostPointer = SpielfeldEintrag::Leer;
			GhostPointer = nullptr;
		}
	}

	switch (Spielfeld[QuelleY][QuelleX])
	{
	case SpielfeldEintrag::WeissBauer:
#pragma region WeissBauer

		if (QuelleY == 1 &&
			Spielfeld[QuelleY + 1][QuelleX] == SpielfeldEintrag::Leer &&
			Spielfeld[QuelleY + 2][QuelleX] == SpielfeldEintrag::Leer &&
			ZielY == 3 &&
			QuelleX == ZielX) // doppelmove erlaubt
		{
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
			Spielfeld[QuelleY + 1][QuelleX] = SpielfeldEintrag::WeissBauerGeist;
			Spielfeld[QuelleY + 2][QuelleX] = SpielfeldEintrag::WeissBauer;
			GhostPointer = &(Spielfeld[QuelleY + 1][QuelleX]);
			// darf sich positiv auf Y achse zwei felder bewegen (ghost mode)
		}
		else if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::Leer && // darf sich positiv auf Y achse ein feld bewegen
			QuelleY + 1 == ZielY &&
			QuelleX == ZielX)
		{
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
			Spielfeld[ZielY][ZielX] = SpielfeldEintrag::WeissBauer;
		}
		else if (QuelleY + 1 == ZielY &&// schlagen
			abs(ZielX - QuelleX) == 1 &&
			Spielfeld[ZielY][ZielX] != SpielfeldEintrag::Leer)
		{
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::SchwarzBauerGeist)
			{
				Spielfeld[QuelleY][ZielX] = SpielfeldEintrag::Leer;
			}
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::SchwarzKoenig) return Spielzug::SchachMatt;

			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
			Spielfeld[ZielY][ZielX] = SpielfeldEintrag::WeissBauer;
		}
		else
			return Spielzug::Illegal;

		if (ZielY == 7)
		{
			return Spielzug::FigurenTausch;
		}



		// darf sich positiv auf Y achse ein feld und ein feld x (absolut) bewegen wenn dort ein gegner ist
		// wenn Y achse 7 erreicht wird er zu einer SpielfeldEintrag::WeissDame
#pragma endregion
		break;
	case SpielfeldEintrag::WeissTurm:
#pragma region WeissTurm
		//  wenn Turm -> nur horizontal oder vertikal, solange nix im weg steht
		if ((QuelleX == ZielX && QuelleY != ZielY) ||  // Vertikaler Move
			(QuelleY == ZielY && QuelleX != ZielX))
		{
			uint8_t queryX = QuelleX;
			uint8_t queryY = QuelleY;

			while (queryX != ZielX || queryY != ZielY)
			{
				if (queryX < ZielX) queryX++;// zählen X
				else if (queryX > ZielX) queryX--;// zählen X

				if (queryY < ZielY) queryY++;// zählen Y
				else if (queryY > ZielY) queryY--;

				if (Spielfeld[queryY][queryX] != SpielfeldEintrag::Leer &&
					(queryX != ZielX || queryY != ZielY))// feld nicht leer?
					return Spielzug::Illegal;
			}
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::SchwarzKoenig) return Spielzug::SchachMatt;
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
			Spielfeld[ZielY][ZielX] = SpielfeldEintrag::WeissTurm;

		}
		else
			return Spielzug::Illegal;
#pragma endregion
		break;
	case SpielfeldEintrag::WeissSpringer:
#pragma region WeissSpringer



		if ((abs(QuelleX - ZielX) == 2 && abs(QuelleY - ZielY) == 1) ||// wenn x entfernung 2 ist und y entfernung 1
			(abs(QuelleY - ZielY) == 2 && abs(QuelleX - ZielX) == 1))// wenn y entfernung 2 ist und x entfernung 1	
		{
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::SchwarzKoenig) return Spielzug::SchachMatt;
			Spielfeld[ZielY][ZielX] = Spielfeld[QuelleY][QuelleX];
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
		}
		else
		{
			return Spielzug::Illegal;
		}
#pragma endregion
		break;
	case SpielfeldEintrag::WeissLaufer:
#pragma region WeissLaufer

		if ((ZielX - QuelleX == ZielY - QuelleY) ||    // Diagonal1
			(QuelleX - ZielX == ZielY - QuelleY))      // Diagnonal2
		{
			uint8_t queryX = QuelleX;
			uint8_t queryY = QuelleY;

			while (queryX != ZielX || queryY != ZielY)
			{
				if (queryX < ZielX) queryX++;// zählen X
				else if (queryX > ZielX) queryX--;// zählen X

				if (queryY < ZielY) queryY++;// zählen Y
				else if (queryY > ZielY) queryY--;

				if (Spielfeld[queryY][queryX] != SpielfeldEintrag::Leer &&
					(queryX != ZielX || queryY != ZielY))// feld nicht leer?
					return Spielzug::Illegal;
			}
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::SchwarzKoenig) return Spielzug::SchachMatt;
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
			Spielfeld[ZielY][ZielX] = SpielfeldEintrag::WeissLaufer;

		}
		else
			return Spielzug::Illegal;
#pragma endregion 
		break;
	case SpielfeldEintrag::WeissDame:
#pragma region WeissDame

		if ((QuelleX == ZielX && QuelleY != ZielY) ||  // Vertikaler Move
			(QuelleY == ZielY && QuelleX != ZielX) ||  // horizontal
			(ZielX - QuelleX == ZielY - QuelleY) ||    // Diagonal1
			(QuelleX - ZielX == ZielY - QuelleY))      // Diagnonal2
		{
			uint8_t queryX = QuelleX;
			uint8_t queryY = QuelleY;

			while (queryX != ZielX || queryY != ZielY)
			{
				if (queryX < ZielX) queryX++;// zählen X
				else if (queryX > ZielX) queryX--;// zählen X

				if (queryY < ZielY) queryY++;// zählen Y
				else if (queryY > ZielY) queryY--;

				if (Spielfeld[queryY][queryX] != SpielfeldEintrag::Leer &&
					(queryX != ZielX || queryY != ZielY))// feld nicht leer?
					return Spielzug::Illegal;
			}
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::SchwarzKoenig) return Spielzug::SchachMatt;
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
			Spielfeld[ZielY][ZielX] = SpielfeldEintrag::WeissDame;

		}
		else
			return Spielzug::Illegal;

#pragma endregion
		break;
	case SpielfeldEintrag::WeissKoenig:
#pragma region WeissKoenig
		if (ZielX > QuelleX - 2 && ZielX < QuelleX + 2 && //  wenn König -> nur ein feld in jede richtung
			ZielY > QuelleY - 2 && ZielY < QuelleY + 2)
		{
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::SchwarzKoenig) return Spielzug::SchachMatt;
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;//		quelle auf leer ändern
			Spielfeld[ZielY][ZielX] = SpielfeldEintrag::WeissKoenig;//		ziel auf könig ändern
		}
		else
			return Spielzug::Illegal;
		//TODO:rochade -> der König kann mit Turm getauscht werden wenn zwischen ihnen platz ist und sich beide noch nicht bewegt haben
#pragma endregion
		break;
	case SpielfeldEintrag::SchwarzBauer:
#pragma region SchwarzBauer
		if (QuelleY == 6 &&
			Spielfeld[QuelleY - 1][QuelleX] == SpielfeldEintrag::Leer &&
			Spielfeld[QuelleY - 2][QuelleX] == SpielfeldEintrag::Leer &&
			ZielY == 4 &&
			QuelleX == ZielX) // doppelmove erlaubt
		{
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
			Spielfeld[QuelleY - 1][QuelleX] = SpielfeldEintrag::SchwarzBauerGeist;
			Spielfeld[QuelleY - 2][QuelleX] = SpielfeldEintrag::SchwarzBauer;
			GhostPointer = &(Spielfeld[QuelleY - 1][QuelleX]);
			// darf sich positiv auf Y achse zwei felder bewegen (ghost mode)
		}
		else if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::Leer && // darf sich positiv auf Y achse ein feld bewegen
			QuelleY - 1 == ZielY &&
			QuelleX == ZielX)
		{
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
			Spielfeld[ZielY][ZielX] = SpielfeldEintrag::SchwarzBauer;
		}
		else if (QuelleY - 1 == ZielY &&// schlagen
			abs(ZielX - QuelleX) == 1 &&
			Spielfeld[ZielY][ZielX] != SpielfeldEintrag::Leer)
		{
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::WeissBauerGeist)
			{
				Spielfeld[QuelleY][ZielX] = SpielfeldEintrag::Leer;
			}
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::WeissKoenig) return Spielzug::SchachMatt;

			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
			Spielfeld[ZielY][ZielX] = SpielfeldEintrag::SchwarzBauer;
		}
		else
		{
			return Spielzug::Illegal;
		}



		// darf sich positiv auf Y achse ein feld und ein feld x (absolut) bewegen wenn dort ein gegner ist
		// wenn Y achse 7 erreicht wird er zu einer SpielfeldEintrag::WeissDame

		//	wenn bauer -> ziel nur in Y koordinate 1 feld verschieben * (ausser beim ersten mal)
#pragma endregion
		break;
	case SpielfeldEintrag::SchwarzTurm:
#pragma region SchwarzTurm
		if ((QuelleX == ZielX && QuelleY != ZielY) ||  // Vertikaler Move
			(QuelleY == ZielY && QuelleX != ZielX))
		{
			uint8_t queryX = QuelleX;
			uint8_t queryY = QuelleY;

			while (queryX != ZielX || queryY != ZielY)
			{
				if (queryX < ZielX) queryX++;// zählen X
				else if (queryX > ZielX) queryX--;// zählen X

				if (queryY < ZielY) queryY++;// zählen Y
				else if (queryY > ZielY) queryY--;

				if (Spielfeld[queryY][queryX] != SpielfeldEintrag::Leer &&
					(queryX != ZielX || queryY != ZielY))// feld nicht leer?
					return Spielzug::Illegal;
			}
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::WeissKoenig) return Spielzug::SchachMatt;
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
			Spielfeld[ZielY][ZielX] = SpielfeldEintrag::SchwarzTurm;

		}
		else
			return Spielzug::Illegal;
#pragma endregion
		break;
	case SpielfeldEintrag::SchwarzSpringer:
#pragma region SchwarzSpringer
		if ((abs(QuelleX - ZielX) == 2 && abs(QuelleY - ZielY) == 1) ||// wenn x entfernung 2 ist und y entfernung 1
			(abs(QuelleY - ZielY) == 2 && abs(QuelleX - ZielX) == 1))// wenn y entfernung 2 ist und x entfernung 1	
		{
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::WeissKoenig) return Spielzug::SchachMatt;
			Spielfeld[ZielY][ZielX] = Spielfeld[QuelleY][QuelleX];
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
		}
		else
		{
			return Spielzug::Illegal;
		}
#pragma endregion
		break;
	case SpielfeldEintrag::SchwarzLaufer:
#pragma region SchwarzLaufer
		if ((ZielX - QuelleX == ZielY - QuelleY) ||    // Diagonal1
			(QuelleX - ZielX == ZielY - QuelleY))      // Diagnonal2
		{
			uint8_t queryX = QuelleX;
			uint8_t queryY = QuelleY;

			while (queryX != ZielX || queryY != ZielY)
			{
				if (queryX < ZielX) queryX++;// zählen X
				else if (queryX > ZielX) queryX--;// zählen X

				if (queryY < ZielY) queryY++;// zählen Y
				else if (queryY > ZielY) queryY--;

				if (Spielfeld[queryY][queryX] != SpielfeldEintrag::Leer &&
					(queryX != ZielX || queryY != ZielY))// feld nicht leer?
					return Spielzug::Illegal;
			}
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::WeissKoenig) return Spielzug::SchachMatt;
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
			Spielfeld[ZielY][ZielX] = SpielfeldEintrag::SchwarzLaufer;

		}
		else
			return Spielzug::Illegal;
#pragma endregion
		break;
	case SpielfeldEintrag::SchwarzDame:
#pragma region SchwarzDame
		if ((QuelleX == ZielX && QuelleY != ZielY) ||  // Vertikaler Move
			(QuelleY == ZielY && QuelleX != ZielX) ||  // horizontal
			(ZielX - QuelleX == ZielY - QuelleY) ||    // Diagonal1
			(QuelleX - ZielX == ZielY - QuelleY))      // Diagnonal2
		{
			uint8_t queryX = QuelleX;
			uint8_t queryY = QuelleY;

			while (queryX != ZielX || queryY != ZielY)
			{
				if (queryX < ZielX) queryX++;// zählen X
				else if (queryX > ZielX) queryX--;// zählen X

				if (queryY < ZielY) queryY++;// zählen Y
				else if (queryY > ZielY) queryY--;

				if (Spielfeld[queryY][queryX] != SpielfeldEintrag::Leer &&
					(queryX != ZielX || queryY != ZielY))// feld nicht leer?
					return Spielzug::Illegal;
			}
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::WeissKoenig) return Spielzug::SchachMatt;
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;
			Spielfeld[ZielY][ZielX] = SpielfeldEintrag::SchwarzDame;

		}
		else
			return Spielzug::Illegal;
#pragma endregion
		break;
	case SpielfeldEintrag::SchwarzKoenig:
#pragma region SchwarzKoenig
		if (ZielX > QuelleX - 2 && ZielX < QuelleX + 2 && //  wenn König -> nur ein feld in jede richtung
			ZielY > QuelleY - 2 && ZielY < QuelleY + 2)
		{
			if (Spielfeld[ZielY][ZielX] == SpielfeldEintrag::WeissKoenig) return Spielzug::SchachMatt;
			Spielfeld[QuelleY][QuelleX] = SpielfeldEintrag::Leer;//		quelle auf leer ändern
			Spielfeld[ZielY][ZielX] = SpielfeldEintrag::SchwarzKoenig;//		ziel auf könig ändern
		}
		else
			return Spielzug::Illegal;
		//TODO:rochade -> der König kann mit Turm getauscht werden wenn zwischen ihnen platz ist und sich beide noch nicht bewegt haben
#pragma endregion
		break;
	default:
		std::cerr << "Fehler: unbekannter Spielfeldinhalt\n";
		break;
	}

	mZugZaehler++;
	SpielerWeissAmZug = !SpielerWeissAmZug;
	return Spielzug::Konform;
}

void Schachbrett::EinheitenTausch(char pEinheitenName, std::string pZiel)
{
	//TODO: Konvertierung von der Main in einer Funktion abhandeln lassen!
	uint8_t ZielX;

	if (pZiel[0] < 97)
		ZielX = pZiel[0] - 65;
	else
		ZielX = pZiel[0] - 97;

	uint8_t ZielY = pZiel[1] - 49;



	switch (pEinheitenName)
	{
	case 'D':
	case 'd':
		Spielfeld[ZielY][ZielX] = (SpielerWeissAmZug ? SpielfeldEintrag::WeissDame : SpielfeldEintrag::SchwarzDame);
		break;
	case 'S':
	case 's':
		Spielfeld[ZielY][ZielX] = (SpielerWeissAmZug ? SpielfeldEintrag::WeissSpringer : SpielfeldEintrag::SchwarzSpringer);
		break;
	case 'L':
	case 'l':
		Spielfeld[ZielY][ZielX] = (SpielerWeissAmZug ? SpielfeldEintrag::WeissLaufer : SpielfeldEintrag::SchwarzLaufer);
		break;
	case 'T':
	case 't':
		Spielfeld[ZielY][ZielX] = (SpielerWeissAmZug ? SpielfeldEintrag::WeissTurm : SpielfeldEintrag::SchwarzTurm);
		break;
	default:
		std::cerr << "Unbakannte gewünschte Einheit\n";
		break;
	}

	SpielerWeissAmZug = !SpielerWeissAmZug;
	mZugZaehler++;
}

void Schachbrett::Draw()
{

	//Überschrift ausgeben
	//Koordinatenachsen
	system("cls");

	std::cout << "Schachspiel ZugNr.: " << mZugZaehler << "\n";
	std::cout << " A B C D E F G H\n";
	for (size_t Y = 0; Y < 8; Y++)
	{
		std::cout << Y + 1;
		for (size_t X = 0; X < 8; X++)
		{
			if ((X + Y) % 2 == 0)
			{
				std::cout << ColorCode(Color::BGWhite);
			}
			else
			{
				std::cout << ColorCode(Color::BGBlack);
			}

			switch (Spielfeld[Y][X])
			{
			case SpielfeldEintrag::SchwarzBauerGeist:
				std::cout << ColorCode(Color::Red) << "G ";
				break;
			case SpielfeldEintrag::WeissBauerGeist:
				std::cout << ColorCode(Color::Yellow) << "G ";
				break;
			case SpielfeldEintrag::Leer:
				std::cout << "  ";
				break;
			case SpielfeldEintrag::WeissBauer:
				std::cout << ColorCode(Color::Yellow) << "B ";
				break;
			case SpielfeldEintrag::WeissTurm:
				std::cout << ColorCode(Color::Yellow) << "T ";
				break;
			case SpielfeldEintrag::WeissSpringer:
				std::cout << ColorCode(Color::Yellow) << "S ";
				break;
			case SpielfeldEintrag::WeissLaufer:
				std::cout << ColorCode(Color::Yellow) << "L ";
				break;
			case SpielfeldEintrag::WeissDame:
				std::cout << ColorCode(Color::Yellow) << "D ";
				break;
			case SpielfeldEintrag::WeissKoenig:
				std::cout << ColorCode(Color::Yellow) << "K ";
				break;
			case SpielfeldEintrag::SchwarzBauer:
				std::cout << ColorCode(Color::Red) << "B ";
				break;
			case SpielfeldEintrag::SchwarzTurm:
				std::cout << ColorCode(Color::Red) << "T ";
				break;
			case SpielfeldEintrag::SchwarzSpringer:
				std::cout << ColorCode(Color::Red) << "S ";
				break;
			case SpielfeldEintrag::SchwarzLaufer:
				std::cout << ColorCode(Color::Red) << "L ";
				break;
			case SpielfeldEintrag::SchwarzDame:
				std::cout << ColorCode(Color::Red) << "D ";
				break;
			case SpielfeldEintrag::SchwarzKoenig:
				std::cout << ColorCode(Color::Red) << "K ";
				break;
			default:
				std::cout << "Unbekannte Daten im Spielfeld :(";
				break;
			}
			std::cout << ColorCode(Color::Reset);
		}
		std::cout << "\n";
	}

	//inhalt des Feldes
	//Abschluss des Spielfeldes

}

void Schachbrett::Reset()
{
	Spielfeld[0][0] = SpielfeldEintrag::WeissTurm;
	Spielfeld[0][1] = SpielfeldEintrag::WeissSpringer;
	Spielfeld[0][2] = SpielfeldEintrag::WeissLaufer;
	Spielfeld[0][3] = SpielfeldEintrag::WeissDame;
	Spielfeld[0][4] = SpielfeldEintrag::WeissKoenig;
	Spielfeld[0][5] = SpielfeldEintrag::WeissLaufer;
	Spielfeld[0][6] = SpielfeldEintrag::WeissSpringer;
	Spielfeld[0][7] = SpielfeldEintrag::WeissTurm;

	for (size_t i = 0; i < 8; i++) Spielfeld[1][i] = SpielfeldEintrag::WeissBauer;

	for (size_t j = 2; j < 6; j++)	for (size_t i = 0; i < 8; i++) Spielfeld[j][i] = SpielfeldEintrag::Leer;

	for (size_t i = 0; i < 8; i++) Spielfeld[6][i] = SpielfeldEintrag::SchwarzBauer;

	Spielfeld[7][0] = SpielfeldEintrag::SchwarzTurm;
	Spielfeld[7][1] = SpielfeldEintrag::SchwarzSpringer;
	Spielfeld[7][2] = SpielfeldEintrag::SchwarzLaufer;
	Spielfeld[7][3] = SpielfeldEintrag::SchwarzDame;
	Spielfeld[7][4] = SpielfeldEintrag::SchwarzKoenig;
	Spielfeld[7][5] = SpielfeldEintrag::SchwarzLaufer;
	Spielfeld[7][6] = SpielfeldEintrag::SchwarzSpringer;
	Spielfeld[7][7] = SpielfeldEintrag::SchwarzTurm;

	SpielerWeissAmZug = true;
}

#ifdef _DEBUG
void Schachbrett::Reset(const SaveGame pID)
{
	for (size_t j = 0; j < 8; j++)	for (size_t i = 0; i < 8; i++) Spielfeld[j][i] = SpielfeldEintrag::Leer;

	switch (pID)
	{
	case SaveGame::FigurenTausch:
		Spielfeld[6][1] = SpielfeldEintrag::WeissBauer;
		Spielfeld[1][5] = SpielfeldEintrag::SchwarzKoenig;
		break;
	case SaveGame::SchachMatt:
		Spielfeld[4][4] = SpielfeldEintrag::SchwarzKoenig;
		Spielfeld[3][5] = SpielfeldEintrag::WeissBauer;
		Spielfeld[2][2] = SpielfeldEintrag::WeissLaufer;
		Spielfeld[5][4] = SpielfeldEintrag::WeissKoenig;
		Spielfeld[6][6] = SpielfeldEintrag::WeissDame;
		break;
	default:
		std::cerr << "Unbekanntes Test-Szenario\n";
		break;
	}
}
#endif // DEBUG

std::string Schachbrett::ColorCode(Color pColor)
{
	switch (pColor)
	{
	case Color::Green:
		return "\x1b[32m";
	case Color::Red:
		return "\x1b[31m";
	case Color::Blue:
		return "\x1b[34m";
	case Color::Yellow:
		return "\x1b[33m";
	case Color::Reset:
		return "\033[0m";
	case Color::BGBlack:
		return "\x1b[40m";
	case Color::BGWhite:
		return "\x1b[47m";
	default:
		std::cerr << "Farbe nicht unterstützt";
		return "\x1b[37m";
	}
}