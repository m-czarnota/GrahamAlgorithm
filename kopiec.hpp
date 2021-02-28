#pragma once
#include <utility>
#include <assert.h>
#include <algorithm>
#include "tablica.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

using namespace std;

template <typename typ>
class Kopiec {
	Tablica <typ>* tablica;
	typ* wstrzyknieta;
	int wstrzyknietaRozmiar;

	void przekopcowywanie_gora(const int& obecnyIndex, const bool (*comparator)(const typ&, const typ&), typ* tab, const int& size) {
		if (size > 1) {
			int rodzic = (obecnyIndex - 1) / 2;
			if (obecnyIndex > 0 && comparator(tab[rodzic], tab[obecnyIndex])) {
				swap(tab[obecnyIndex], tab[rodzic]);
				przekopcowywanie_gora(rodzic, comparator, tab, size);
			}
		}
	}
	void przekopcowywanie_gora(const int& obecnyIndex, const bool (*comparator)(const typ&, const typ&), Tablica<typ>* tab, const int& size) {
		if (size > 1) {
			int rodzic = (obecnyIndex - 1) / 2;
			if (obecnyIndex > 0 && comparator(tab->operator[](rodzic), tab->operator[](obecnyIndex))) {
				swap(tab->operator[](obecnyIndex), tab->operator[](rodzic));
				przekopcowywanie_gora(rodzic, comparator, tab, size);
			}
		}
	}

	void przekopcowywanie_dol(const int& obecnyIndex, const bool (*comparator)(const typ&, const typ&), typ* tab, const int& size) {
		if (size > 1) {
			if (obecnyIndex < size) {
				int lewy = 2 * obecnyIndex + 1;
				int prawy = 2 * obecnyIndex + 2;
				if (lewy < size) {
					bool lewy_wiekszy = false;
					if (prawy < size) {
						if (!comparator(tab[lewy], tab[prawy]))
							lewy_wiekszy = true;
					}
					int index = lewy;
					if (!lewy_wiekszy) {
						if (prawy < size)
							index = prawy;
					}
					if (comparator(tab[obecnyIndex], tab[index])) {
						swap(tab[obecnyIndex], tab[index]);
						przekopcowywanie_dol(index, comparator, tab, size);
					}
				}
			}
		}
	}
	void przekopcowywanie_dol(const int& obecnyIndex, const bool (*comparator)(const typ&, const typ&), Tablica<typ>* tab, const int& size) {
		if (size > 1) {
			if (obecnyIndex < size) {
				int lewy = 2 * obecnyIndex + 1;
				int prawy = 2 * obecnyIndex + 2;
				if (lewy < size) {
					bool lewy_wiekszy = false;
					if (prawy < size) {
						if (!comparator(tab->operator[](lewy), tab->operator[](prawy)))
							lewy_wiekszy = true;
					}
					int index = lewy;
					if (!lewy_wiekszy) {
						if (prawy < size)
							index = prawy;
					}
					if (comparator(tab->operator[](obecnyIndex), tab->operator[](index))) {
						swap(tab->operator[](obecnyIndex), tab->operator[](index));
						przekopcowywanie_dol(index, comparator, tab, size);
					}
				}
			}
		}
	}

public:
	Kopiec() {
		tablica = new Tablica<typ>();
		wstrzyknieta = nullptr;
		wstrzyknietaRozmiar = 0;
	}

	Kopiec(typ* tab, const int& size, const bool (*comparator)(const typ&, const typ&), const int& gora = false) {
		tablica = nullptr;
		wstrzyknieta = tab;
		wstrzyknietaRozmiar = size;
		if (!gora) {
			for (int i = size - 1; i >= 0; i--)
				przekopcowywanie_dol(i, comparator, wstrzyknieta, wstrzyknietaRozmiar);
		}
		else {
			for (int i = size - 1; i >= 0; i--)
				przekopcowywanie_gora(i, comparator, wstrzyknieta, wstrzyknietaRozmiar);
		}
	}

	~Kopiec() {
		delete tablica;
	}

	Kopiec(Kopiec&& item) {
		this->tablica = move(item.tablica);
	}
	Kopiec(const Kopiec& item) {
		this->tablica = item.tablica;
	}

	Kopiec& operator=(const Kopiec& item) {
		this->tablica = item.tablica;
		return *this;
	}
	Kopiec& operator=(Kopiec&& item) {
		this->tablica = move(item.tablica);
		return *this;
	}

	friend ostream& operator<<(ostream& outgoing, Kopiec<typ>& item) {
		outgoing << "Elementy kopca:" << endl;
		int ile = ((item.tablica->get_size() > 10) ? 10 : item.tablica->get_size());
		for (int i = 0; i < ile; i++) {
			outgoing << "Indeks nr." << i << ", dane: ";
			outgoing << item.tablica->operator[](i) << endl;
		}
		return outgoing;
	}

	friend ostream& operator<<(ostream& outgoing, Kopiec<typ>* item) {
		if (item->tablica->get_size() < 1) {
			outgoing << "Kopiec jest pusty!" << endl;
			return outgoing;
		}
		outgoing << "Ilosc elementow kopca: " << item->tablica->get_size() << endl;
		outgoing << "Elementy kopca:" << endl;
		int ile = ((item->tablica->get_size() > 10) ? 10 : item->tablica->get_size());
		for (int i = 0; i < ile; i++) {
			outgoing << i << ": [" << item->tablica->operator[](i) << "]";
			if ((2 * i + 1) < item->tablica->get_size())
				outgoing << ", lewy: " << item->tablica->operator[](2 * i + 1);
			if ((2 * i + 2) < item->tablica->get_size())
				outgoing << ", prawy: " << item->tablica->operator[](2 * i + 2);
			if (((i - 1) / 2) >= 0 && i > 0)
				outgoing << ", rodzic: " << item->tablica->operator[]((i - 1) / 2);
			outgoing << endl;
		}
		return outgoing;
	}

	//------------------------- metody interfejsu -------------------------

	void dodaj(const typ& item, const bool (*comparator)(const typ&, const typ&)) {
		tablica->dodaj(item);
		przekopcowywanie_gora(tablica->get_size() - 1, comparator, tablica, tablica->get_size());
	}

	const typ deleteKorzen(const bool (*comparator)(const typ&, const typ&)) {
		int size = wstrzyknietaRozmiar;
		if (!wstrzyknieta)
			size = tablica->get_size();
		if (!size)
			return 0;
		int index = 0;
		typ temp;
		auto normalnaTablica = [&] {
			temp = move(tablica->operator[](index));
			swap(tablica->operator[](index), tablica->operator[](tablica->get_size() - 1));
			typ object = move(tablica->operator[](tablica->get_size() - 1));
			tablica->get_size(1);
			przekopcowywanie_dol(index, comparator, tablica, tablica->get_size());
			if (tablica->get_size() < 0)
				deleteAll();
		};
		auto wstrzyknietaTablica = [&] {
			temp = move(wstrzyknieta[index]);
			swap(wstrzyknieta[index], wstrzyknieta[wstrzyknietaRozmiar - 1]);
			typ object = move(wstrzyknieta[wstrzyknietaRozmiar - 1]);
			wstrzyknietaRozmiar--;
			przekopcowywanie_dol(index, comparator, wstrzyknieta, wstrzyknietaRozmiar);
			if (wstrzyknietaRozmiar < 0)
				delete[] wstrzyknieta;
		};
		if (!wstrzyknieta)
			normalnaTablica();
		else
			wstrzyknietaTablica();
		return temp;
	}

	void sortowanie(const bool (*comparator)(const typ&, const typ&)) {
		for (int i = wstrzyknietaRozmiar - 1; i >= 0; i--) {
			swap(wstrzyknieta[0], wstrzyknieta[i]);
			przekopcowywanie_dol(0, comparator, wstrzyknieta, i);
		}
	}

	void deleteAll() {
		for (int i = 0; i < tablica->get_size(); i++)
			typ object = move(tablica->operator[](i));
		delete tablica;
		tablica = nullptr;
		tablica = new Tablica<typ>;
	}

	void shortNotation() {
		cout << "Ilosc elementow w kopcu: " << tablica->get_size() << endl;
	}
};