#include <iostream>
#include "menu.h"
using namespace std;


int showMenu() {
	int ans;
	cout << "==============================================================" << endl;
	cout << "1. Wczytaj nowe dane z pliku " << endl;
	cout << "2. Wprowadz kryterium stopu " << endl;
	cout << "3. Ustaw wielkosc populacji poczatkowej" << endl;
	cout << "4. Ustaw wspolczynnik mutacji" << endl;
	cout << "5. Ustaw wspolczynnik krzyzowania" << endl;
	cout << "6. Wybierz metode krzyzowania" << endl;
	cout << "7. Wybierz metode mutacji" << endl;
	cout << "8. Uruchom algorytm" << endl;
	cout << "0. ZAKONCZ" << endl;
	cout << "==============================================================" << endl;
	cin >> ans;

	return ans;
}


int showStopTimeMenu() {
	int stopTime;
	cout << "==============================================================" << endl;
	cout << "Wprowadz kryterium stopu (czas w sekundach): " << endl;
	cout << "==============================================================" << endl;
	cin >> stopTime;
	return stopTime;
}

int showPopMenu() {
	int pop;
	cout << "==============================================================" << endl;
	cout << "Wprowadz wielkosc populacji początkowej: " << endl;
	cout << "==============================================================" << endl;
	cin >> pop;
	return pop;
}

double showMutPercMenu() {
	double mut;
	cout << "==============================================================" << endl;
	cout << "Wprowadz wspolczynnik mutacji: " << endl;
	cout << "==============================================================" << endl;
	cin >> mut;
	return mut;
}

double showXPercMenu() {
	double x;
	cout << "==============================================================" << endl;
	cout << "Wprowadz wspolczynnik krzyzowania: " << endl;
	cout << "==============================================================" << endl;
	cin >> x;
	return x;
}

int showXMenu() {
	int ans;
	cout << "==============================================================" << endl;
	cout << "0.JEdno " << endl;
	cout << "1.Drugie " << endl;
	cout << "==============================================================" << endl;
	cin >> ans;
	return ans;
}

int showMMenu() {
	int ans;
	cout << "==============================================================" << endl;
	cout << "0.JEdno " << endl;
	cout << "1.Drugieee " << endl;
	cout << "==============================================================" << endl;
	cin >> ans;
	return ans;
}