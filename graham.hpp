#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include "reprezentanci.hpp"

Punkt pkt_min{ 0,0 };

static int comparator_sort(const void* item1, const void* item2) {
	std::pair<int, Punkt> pkt1 = *static_cast<const std::pair<int, Punkt>*>(item1);
	std::pair<int, Punkt> pkt2 = *static_cast<const std::pair<int, Punkt>*>(item2);
	int orientacja = Punkt_orientacja(pkt_min, pkt1.second, pkt2.second);
	if (!orientacja) {
		double odleglosc2 = Punkt_odleglosc(pkt_min, pkt2.second),
			odleglosc1 = Punkt_odleglosc(pkt_min, pkt1.second);
		if (odleglosc2 >= odleglosc1)
			return -1;
		return 1;
	}
	if (orientacja == -1)
		return -1;
	return 1;
}

std::vector<int> algorytmGrahama(const std::vector<Punkt>& punkty, double* czasSortowania = nullptr, double* czasPetli = nullptr) {
	int pkt_indexMin = 0;
	for (int i = 0; i < punkty.size(); i++) {
		if (punkty[i] < punkty[pkt_indexMin])
			pkt_indexMin = i;
	}

	std::vector<std::pair<int, Punkt>> punkty_kopia;
	for (int i = 0; i < punkty.size(); i++) {
		auto para = std::make_pair(i, punkty[i]);
		punkty_kopia.push_back(para);
	}
	std::swap(punkty_kopia[0], punkty_kopia[pkt_indexMin]);
	pkt_min = punkty_kopia[0].second;

	clock_t time1 = clock();
	std::qsort(&punkty_kopia[1], punkty_kopia.size() - 1, sizeof(std::pair<int, Punkt>), comparator_sort);
	clock_t time2 = clock();
	if (czasSortowania)
		*czasSortowania = (time2 - time1) / (double)CLOCKS_PER_SEC;

	std::vector<int> indexy; //indexy do zwrócenia z otoczki wypuk³ej
	for (auto& item : punkty_kopia)
		indexy.push_back(item.first);

	//punkty le¿¹ce wzd³u¿ jednej lini
	int size_changed = 1;
	for (int i = 1; i < punkty_kopia.size(); i++) {
		while (i < (punkty_kopia.size() - 1) &&
			Punkt_orientacja(pkt_min, punkty_kopia[i].second, punkty_kopia[i + 1].second) == 0)
			i++;

		punkty_kopia[size_changed] = punkty_kopia[i];
		size_changed++;
	}
	if (size_changed < 3) //zbyt ma³o punktów do otoczki wypuk³ej
		return indexy; //je¿eli nie da siê stworzyæ otoczki wypuk³ej to zwróæ pierwotne indexy

	std::vector<Punkt> powloka;
	indexy.clear(); //wyczyszczenie indexów do zwrócenia
	powloka.push_back(punkty_kopia[0].second); //najmniejszy na poczatek
	indexy.push_back(punkty_kopia[0].first);
	powloka.push_back(punkty_kopia[1].second); //pierwszy z posortowanych
	indexy.push_back(punkty_kopia[1].first);
	time1 = clock();
	for (size_t i = 2; i < size_changed; i++) { //od drugiego z posortowanych
		while (powloka.size() > 2 && Punkt_orientacja(powloka[powloka.size() - 2], powloka[powloka.size() - 1], punkty_kopia[i].second) != -1) {
			powloka.erase(powloka.end() - 1);
			indexy.erase(indexy.end() - 1);
		}
		powloka.push_back(punkty_kopia[i].second);
		indexy.push_back(punkty_kopia[i].first);

	}
	time2 = clock();
	if (czasPetli)
		*czasPetli = (time2 - time1) / (double)CLOCKS_PER_SEC;
	powloka.clear();

	return indexy;
}