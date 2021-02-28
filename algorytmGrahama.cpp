#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "graham.hpp"

int main()
{
	/*{ //specjalny test Klimaszewskiego
		std::vector<Punkt> p{ Punkt{0,0},{-1,0},{1,0},{0,-1},{0,1}, {-0.5,-0.5},{0.5,0.5},{-0.5,0.5},{0.5,-0.5} };
		for (const int& i : algorytmGrahama(p))
			std::cout << i << " ";
		return 0;
	}*/
	const int n = 5;
	for (size_t i = 1; i <= n; i++) {
		std::vector<Punkt> punkty;
		std::fstream plik;
		std::string sciezka = "excercises\\points";
		sciezka += std::to_string(i);
		sciezka += ".txt";

		int ileLini = 0, ileZebrano = 0;
		plik.open(sciezka, std::ios::in);
		if (plik.is_open()) {
			while (!plik.eof()) {
				std::string line;
				getline(plik, line);
				if (line == "") //to dlatego, ¿e na koñcu plików znajduje siê pusta linia, nie jest to koniec pliku, jednak std::stoi generuje b³¹d
					break;
				double dane1 = 0;
				if (!ileLini || ileZebrano == ileLini) { //czy ju¿ wie ile ma odczytaæ lini poni¿ej lub czy ju¿ zebra³o
					ileLini = std::stoi(line); //ile lini ma odczytaæ poni¿ej
					ileZebrano = 0; //ile zebra³o lini zerowanie

					continue; //dalej nic ma nie do odczytu w tej lini
				}

				std::string wyraz = ""; //tu zapisywane znalezione wyrazy
				for (auto& literka : line) {
					if (literka == ' ') {
						dane1 = std::stod(wyraz);
						wyraz = "";
						continue;
					}
					wyraz += literka;
				}

				double dane2 = std::stod(wyraz);
				Punkt pkt(dane1, dane2);
				punkty.emplace_back(pkt);

				++ileZebrano; //ile lini ju¿ zebra³o
			}
			plik.close();
			//koniec operacji na pliku, ale dalsza czêœæ nie mo¿e siê staæ, je¿li plik nie zostanie poprawnie wczytany

			double czasSortowania, czasPetli;
			std::vector<int> graham = algorytmGrahama(punkty, &czasSortowania, &czasPetli);

			std::cout << "Liczba punktow wejsciowych: " << punkty.size() << std::endl;
			std::cout << "Liczba punktow w powloce wypuklej: " << graham.size() << std::endl;
			std::cout << "--------- Punkty w powloce wypuklej ---------" << std::endl;
			for (auto& item : graham)
				std::cout << item << "; ";
			std::cout << std::endl << "--------- Koniec ---------" << std::endl;
			std::cout << "Czas sortowania: " << czasSortowania << " sek" << std::endl;
			std::cout << "Czas glownej petli: " << czasPetli << " sek" << std::endl;
			std::cout << std::endl << std::endl;

			graham.clear();
		}
		else
			std::cout << "Blad otwarcia pliku " << sciezka << std::endl;

		punkty.clear();
	}
}