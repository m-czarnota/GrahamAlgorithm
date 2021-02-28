#pragma once
#include <iostream>
#include <utility>

struct Punkt {
	double m_x, m_y;

	Punkt(const double& x, const double& y)
		: m_x(x), m_y(y) {}
	~Punkt() {}

	Punkt(const Punkt& item)
		: m_x(item.m_x), m_y(item.m_y) {}
	Punkt& operator=(const Punkt& item) {
		this->m_x = item.m_x;
		this->m_y = item.m_y;
		return *this;
	}
	Punkt(const Punkt&& item)
		: m_x(std::move(item.m_x)), m_y(std::move(item.m_y)) {}
	Punkt& operator=(const Punkt&& item) {
		this->m_x = std::move(item.m_x);
		this->m_y = std::move(item.m_y);
		return *this;
	}

	const bool operator<(const Punkt& item) const {
		if (this->m_y == item.m_y)
			return this->m_x < item.m_x;
		return this->m_y < item.m_y;
	}

	friend std::ostream& operator<<(std::ostream& output, const Punkt& item) {
		output << "(" << item.m_x << ",";
		output << item.m_y << ")";
		return output;
	}
};
const int Punkt_orientacja(const Punkt& item1, const Punkt& item2, const Punkt& item3) {
	double liczba = ((item2.m_y - item1.m_y) * (item3.m_x - item2.m_x)) -
		((item2.m_x - item1.m_x) * (item3.m_y - item2.m_y));
	if (liczba > 0)
		return 1;
	if (liczba < 0)
		return -1;
	return 0;
}
const double Punkt_odleglosc(const Punkt& item1, const Punkt& item2) {
	double odleglosc = ((item1.m_x - item2.m_x) * (item1.m_x - item2.m_x)) +
		((item1.m_y - item2.m_y) * (item1.m_y - item2.m_y));
	return odleglosc;
}