#include <iostream>
/*#include <stdio.h>
#include <io.h>
#include <fcntl.h>*/
#include "Schachbrett.h"

int main()
{
	//_setmode(_fileno(stdout), _O_U16TEXT);
	Schachbrett meinSchachbrett;
	meinSchachbrett.Reset(SaveGame::SchachMatt);
	std::string ZugQuelle;
	std::string ZugZiel;
	Spielzug ZugErgebnis;
	meinSchachbrett.Draw();
	while (true)
	{
		do
		{
			if (meinSchachbrett.SpielerWeissAmZug)
			{
				std::cout << "Spieler Weiss Figur wählen: ";
				std::cin >> ZugQuelle;
				std::cout << "Spieler Weiss Ziel wählen: ";
				std::cin >> ZugZiel;
			}
			else
			{
				std::cout << "Spieler Schwarz Figur wählen: ";
				std::cin >> ZugQuelle;
				std::cout << "Spieler Schwarz Ziel wählen: ";
				std::cin >> ZugZiel;
			}

			ZugErgebnis = meinSchachbrett.Zug(ZugQuelle, ZugZiel);

		} while (ZugErgebnis == Spielzug::Illegal);

		std::string gewuenschteFigur;
		switch (ZugErgebnis)
		{
		case Spielzug::Patt:
			std::cout << "Remis / Patt";
			return 0;
		case Spielzug::SchachMatt:
			if (meinSchachbrett.SpielerWeissAmZug)
				std::cout << "Spielende durch Sieg von Weiss";
			else
				std::cout << "Spielende durch Sieg von Schwarz";
			return 0;
		case Spielzug::FigurenTausch:

			do
			{
				std::cout << "Welche Figur möchten sie haben (DLTS)";

				std::cin >> gewuenschteFigur;
			} while (gewuenschteFigur.length() != 1 ||
				(gewuenschteFigur[0] != 'D' && gewuenschteFigur[0] != 'L' && gewuenschteFigur[0] != 'T' && gewuenschteFigur[0] != 'S' &&
				gewuenschteFigur[0] != 'd' && gewuenschteFigur[0] != 'l' && gewuenschteFigur[0] != 't' && gewuenschteFigur[0] != 's')
				);

			meinSchachbrett.EinheitenTausch(gewuenschteFigur[0], ZugZiel);

		default:
			meinSchachbrett.Draw();
			if (ZugErgebnis == Spielzug::Schach)
			{
				std::cout << meinSchachbrett.ColorCode(Color::Red) << "Vorsicht Schach \n" << meinSchachbrett.ColorCode(Color::Reset);
			}
		}


	}

	return 0;
}
