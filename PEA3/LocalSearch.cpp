#include "LocalSearch.h"
#include <iostream>
#include <random>
#include <ctime>
#include <chrono>
#include <fstream>
#include "Czas.h"

using namespace std;

LocalSearch::LocalSearch() {
}

void LocalSearch::setMatrix(int** tsp, int s) {
	tspMatrix = tsp;
	matrixSize = s;
}

void LocalSearch::setSA(double c,int t) {
	initAlpha = 0.5;
	cooling = c;
	timeLimit = t;
}

void LocalSearch::setTS(int time, int neig)
{
	timeLimit = time;
	tabuCadence = matrixSize/4;
	iterLimit = matrixSize;
	neighborType = neig;
			
}


int LocalSearch::calcCost(vector<unsigned> v)
{
	int s = 0;
	int a, b;
	for (int i = 0; i < matrixSize; i++) {
		a = v.at(i);
		b = v.at(i + 1);
		//cout << "I= " << i << " j = " << j << endl;
		s += tspMatrix[a][b];
		//cout << matrix[i][j] << " ";
	}
	return s;
}

void LocalSearch::showRoute(vector <unsigned> v)
{
	for (int i = 0; i < v.size()-1; i++)
		cout << v.at(i) << "->";
	cout << v.at(v.size()-1);
	cout << endl;
}

int LocalSearch::getInitGreedy()
{
	routeTab.clear();
	int bestMin;
	int tempBest = 0;
	int oldTempBest = 0;
	int localMin = 0;
	int* visitedTab = new int[matrixSize];
	for (int i = 0; i < matrixSize; i++)
	{
		visitedTab[i] = 0;
	}

	bool visited;
	for (int i = 0; i < matrixSize;i++)
	{
		bestMin = INT_MAX;
		oldTempBest = tempBest;
			for (int k = 0; k < matrixSize; k++) {
				visited = true;
				if (k != oldTempBest) {
					for (int o = 0; o <= i; o++) {
						if (k == visitedTab[o]) {
							visited = false;
						}
					}
					if (tspMatrix[oldTempBest][k] < bestMin && visited == true) {
						bestMin = tspMatrix[oldTempBest][k];
						tempBest = k;
					}
				}
			}
		
		if (i < (matrixSize - 1)) {
			localMin += bestMin;
		}
		else {
			localMin += tspMatrix[oldTempBest][0];
		}
		routeTab.push_back(oldTempBest);
		visitedTab[i] = tempBest;
	}
	routeTab.push_back(0);
	return localMin;
}


void LocalSearch::startSA() {
	valueForTime.clear();
	best = INT_MAX;
	iterations = 0;
	// Przypisanie drogi poczatkowej
	optimalMin = getInitGreedy();
	cout << optimalMin << endl;
	showRoute(routeTab);
	cout << "============" << endl;
	/////////////////////////////////////
	initTemp = optimalMin * initAlpha;
	/// /////////////////
	vector <unsigned> shuffled;
	currentRoute = routeTab;
	int currentCost = optimalMin;
	temp = initTemp;
	iterLimit = 10 * matrixSize;
	int dif;
	currentOptimalMin = optimalMin;
	
	//Inicjalizacja zegarów 
	Czas timerTen;
	timerTen.czasStart();
	timerSOL.czasStart();
	timerSA.czasStart();
	do
	{
		for (int i = 0; i < iterLimit; i++)
		{
			// Przetasowanie trasy
			shuffled = currentRoute;
			dif = shufflePath(shuffled);
			currentCost += dif;

			if (currentCost < currentOptimalMin)	//sprawdzenie kosztu nowej
			{
				currentRoute = shuffled;
				currentOptimalMin = currentCost;
				if (currentCost < best) {			//Zapis najlepszego 
					best = currentCost;
					timerSOL.czasStop();
				}
			}
			else if (moveOrNo(currentOptimalMin,currentCost, temp)) {	//przyjecie gorszego rozwiazania
				currentRoute = shuffled;
				currentOptimalMin = currentCost;
			}
			else {
				currentCost -= dif;
			}
			iterations++;

			//pomiar do sprawozdania (blad w czasie)
			timerTen.czasStop();
			if (timerTen.czasWykonaniaSek() >= 5)
			{
				valueForTime.push_back(best);
				timerTen.czasStart();
			}
		}						//koniec epoki
		temp = changeTemp(temp);	//zmiana temperatury
		//cout << temp << endl;
		//cout << currentCost << endl;
	
	} while (ifContinueT(timerSA));
	routeTab = currentRoute;
	optimalMin = currentOptimalMin;
	cout << "T poczatkowa: " << initTemp << endl;
	cout << "T koncowa: "<<temp << endl;
	//cout << optimalMin << endl;
	cout << "Najlepsza wartosc: " << best << endl;
	showRoute(routeTab);
	

}

int LocalSearch::shufflePath(vector <unsigned>& shuffled)
{
	random_device randomSrc;
	default_random_engine randomGenerator(randomSrc());
	uniform_int_distribution<> randNode(1, matrixSize - 1);
	int x, y;
	int b;

	do {
		x = randNode(randomGenerator);
		y = randNode(randomGenerator);

	} while (x == y || y < x);

	b = swapCalculator(x,y,shuffled);
	vSwap(x, y, shuffled);
	return b;
}

int LocalSearch::swapCalculator(int x, int y, vector <unsigned>currentRoute)
{	
	int bal;
	vector <unsigned> tempR = currentRoute;
	vSwap(x, y, tempR);
	bal = calcCost(tempR) - calcCost(currentRoute);
	/*if (x + 1 == y) {
		bal = 0 - tspMatrix[currentRoute.at(x - 1)][currentRoute.at(x)];
		bal -= tspMatrix[currentRoute.at(x)][currentRoute.at(y)];
		bal -= tspMatrix[currentRoute.at(y)][currentRoute.at(y + 1)];
		bal += tspMatrix[currentRoute.at(x - 1)][currentRoute.at(y)];
		bal += tspMatrix[currentRoute.at(y)][currentRoute.at(x)];
		bal += tspMatrix[currentRoute.at(x)][currentRoute.at(y + 1)];
	}
	else {
		bal = 0 - tspMatrix[currentRoute.at(x - 1)][currentRoute.at(x)];
		bal -= tspMatrix[currentRoute.at(x)][currentRoute.at(x + 1)];
		bal -= tspMatrix[currentRoute.at(y - 1)][currentRoute.at(y)];
		bal -= tspMatrix[currentRoute.at(y)][currentRoute.at(y + 1)];
		bal += tspMatrix[currentRoute.at(x - 1)][currentRoute.at(y)];
		bal += tspMatrix[currentRoute.at(y)][currentRoute.at(x + 1)];
		bal += tspMatrix[currentRoute.at(y - 1)][currentRoute.at(x)];
		bal += tspMatrix[currentRoute.at(x)][currentRoute.at(y + 1)];
	}*/
	return bal;
}

int LocalSearch::insertCalculator(int x, int y, vector <unsigned> currentRoute) {
	int bal;
	vector <unsigned> tempR = currentRoute;
	vInsert(x, y, tempR);
	bal = calcCost(tempR) - calcCost(currentRoute);

	//int bal = 0;
	//bal = 0 - tspMatrix[currentRoute.at(x)][currentRoute.at(x + 1)];
	//bal = bal - tspMatrix[currentRoute.at(y - 1)][currentRoute.at(y)];
	//bal = bal - tspMatrix[currentRoute.at(x - 1)][currentRoute.at(x)];
	//bal = bal + tspMatrix[currentRoute.at(x - 1)][currentRoute.at(x + 1)];
	//bal = bal + tspMatrix[currentRoute.at(y - 1)][currentRoute.at(x)];//!!!!!!!!!!!!!!!!
	//bal = bal + tspMatrix[currentRoute.at(x)][currentRoute.at(y)];
	//return bal;

	/*int bal = 0;
	bal = 0 - tspMatrix[currentRoute.at(y)][currentRoute.at(y + 1)];
	bal = bal - tspMatrix[currentRoute.at(x - 1)][currentRoute.at(x)];
	bal = bal - tspMatrix[currentRoute.at(y - 1)][currentRoute.at(y)];
	bal = bal + tspMatrix[currentRoute.at(y - 1)][currentRoute.at(y + 1)];
	bal = bal + tspMatrix[currentRoute.at(x - 1)][currentRoute.at(y)];
	bal = bal + tspMatrix[currentRoute.at(y)][currentRoute.at(x)];*/
	return bal;
}




bool LocalSearch::moveOrNo(int a, int b, double temp) {
	//srand(time(NULL));
	double probability = exp(-(b - a) / temp);
	double f = (double) rand() / RAND_MAX;
	if (f < probability) return true;
	else return false;
}

double LocalSearch::changeTemp(double temp){
	double newtemp;
	newtemp = cooling * temp;			//redukcja geometryczna

	//newtemp = temp / (1 + cooling * temp);	//powolny spadek

	return newtemp;
}



void LocalSearch::startTS() {
	valueForTime.clear();
	tabuList.clear();
	Czas timer;
	vector <unsigned> tabuRouteTab;
	optimalMin = getInitGreedy();
	//cout << optimalMin << endl;
	//showRoute(routeTab);
	cout << "============" << endl;

	currentRoute = routeTab;

	bool ifContinue = true;

	currentTabuCadence = tabuCadence;
	currentOptimalMin = optimalMin;
	int firstCounter = 0;
	int b = 0, bestB;
	int bestI, bestJ;
	int noImprovement = 0;
	bool firstIters = true;
	//bool ifTabu;
	int tabuIterLimit = iterLimit;
	int iterCounterF = 0;
	int helper1;

	Czas timerTen;
	timerTen.czasStart();
	timer.czasStart();
	timerSOL.czasStart();
	do {
		iterCounterF++;

		vector <unsigned> curTabu;
		curTabu.push_back(currentTabuCadence);
		switch (neighborType)
		{
		case 0:
		{
			bestB = bestSwapNeighbor(bestI, bestJ, currentRoute);
			//cout << "(" << bestI << "," << bestJ << ")" << endl;
			curTabu.push_back(currentRoute[bestI]);
			curTabu.push_back(currentRoute[bestJ]);
			vSwap(bestI, bestJ, currentRoute);
			//showRoute(currentRoute);
			break;
		}
		case 1:
		{
			bestB = bestInsertNeighbor(bestI, bestJ, currentRoute);
			//cout << "(" << bestI << "," << bestJ << ")" << endl;
			curTabu.push_back(currentRoute[bestI]);
			curTabu.push_back(currentRoute[bestJ]);
			vInsert(bestI, bestJ, currentRoute);
			//showRoute(currentRoute);
			break;
		}
		case 2:
		{
			bestB = bestReverseNeighbor(bestI, bestJ, currentRoute);
			//cout << "(" << bestI << "," << bestJ << ")" << endl;
			curTabu.push_back(currentRoute[bestI]);
			curTabu.push_back(currentRoute[bestJ]);
			vReverse(bestI, bestJ, currentRoute);
			//showRoute(currentRoute);
			break;
		}
		default:
		{
			cout << "Blad!!!!!" << endl;
			system("pause");
			break;
		}
		}

		currentOptimalMin += bestB;
		if (currentOptimalMin < optimalMin) {
			noImprovement = 0; //zeruje bo znalazl lepszy wynik
			optimalMin = currentOptimalMin;
			tabuRouteTab = currentRoute;
			timerSOL.czasStop();
			helper1 = iterCounterF;
			/*intens = true;*/
		}

		correctTabuList();	//zmniejszenie kadencji i usuniecie zerowych
		tabuList.push_back(curTabu);
		curTabu.clear();
		noImprovement++;
		//cout << currentOptimalMin << " " << noImprovement <<  endl;
		//showRoute(currentRoute);
		
		if (noImprovement > tabuIterLimit) {//dywersyfikacja
			diversification(currentRoute);
			tabuList.clear();
			//showRoute(currentRoute);
			currentOptimalMin = calcCost(currentRoute);
			currentTabuCadence = tabuCadence;
			noImprovement = 0;
		}
		timerTen.czasStop();
		if (timerTen.czasWykonaniaSek() >= 5)
		{
			valueForTime.push_back(optimalMin);
			timerTen.czasStart();
		}
	} while (ifContinueT(timer));
	//while (1);
	//cout << optimalMin << endl;
	//showRoute(tabuRouteTab);
	cout << "Koszt minimalny: " << calcCost(tabuRouteTab) << endl;
	cout << "Moment znaleznienia: " << timerSOL.czasWykonaniaSek() << endl;
	routeTab = tabuRouteTab;
}


void LocalSearch::vInsert(int a, int b, vector <unsigned>& currentRoute) {
	currentRoute.insert(currentRoute.begin() + a, currentRoute.at(b));
	if (a > b)
		currentRoute.erase(currentRoute.begin() + b);
	else
		currentRoute.erase(currentRoute.begin() + b + 1);
}

void LocalSearch::vSwap(int a, int b, vector <unsigned>& currentRoute) {
	unsigned buffer = currentRoute.at(b);
	currentRoute.at(b) = currentRoute.at(a);
	currentRoute.at(a) = buffer;
}

void LocalSearch::vReverse(int a, int b, vector <unsigned>& currentRoute) {
	reverse(currentRoute.begin() + a, currentRoute.begin() + b + 1);
}

//void LocalSearch::calculateReverse(int i, int j, int& balance, vector <unsigned> currentRoute) {
//
//	balance = 0 - tspMatrix[currentRoute.at(i - 1)][currentRoute.at(i)] - tspMatrix[currentRoute.at(j)][currentRoute.at(j + 1)];
//	balance = balance + tspMatrix[currentRoute.at(i - 1)][currentRoute.at(j)] + tspMatrix[currentRoute.at(i)][currentRoute.at(j + 1)];
//
//	for (int k = i; k < j; k++)
//		balance = balance - tspMatrix[currentRoute.at(k)][currentRoute.at(k + 1)] + tspMatrix[currentRoute.at(k + 1)][currentRoute.at(k)];
//
//}

int LocalSearch::reverseCalculator(int x, int y, vector <unsigned> currentRoute) {
	int bal = 0;
	vector <unsigned> tempR = currentRoute;
	vReverse(x, y, tempR);
	bal = calcCost(tempR) - calcCost(currentRoute);
	/*bal = 0 - tspMatrix[currentRoute.at(x - 1)][currentRoute.at(x)] - tspMatrix[currentRoute.at(y)][currentRoute.at(y + 1)];
	bal += tspMatrix[currentRoute.at(x - 1)][currentRoute.at(y)] + tspMatrix[currentRoute.at(x)][currentRoute.at(y + 1)];

	for (int i = x; i < y; i++) {
		bal -= tspMatrix[currentRoute.at(i)][currentRoute.at(i + 1)];
		bal += tspMatrix[currentRoute.at(i + 1)][currentRoute.at(i)];
	}*/
	return bal;
}

int LocalSearch::bestReverseNeighbor(int &bestI, int &bestJ, vector <unsigned> currentRoute) {
	
	bool ifTabu;
	int b,bestB=INT_MAX;
	bestI = 0;
	bestJ = 0;

	for (int i = 1; i < matrixSize-1;i++) {	///
		for (int j = i+1; j < matrixSize;j++) {///
			b = reverseCalculator(i, j, currentRoute);
			ifTabu = false;
			for (int k = 0; k < tabuList.size();k++) {
				if (tabuList.at(k).at(1) == currentRoute.at(i) && tabuList.at(k).at(2) == currentRoute.at(j))
				{
					ifTabu = true;
					break;
				}
				if (tabuList.at(k).at(1) == currentRoute.at(j) && tabuList.at(k).at(2) == currentRoute.at(i))
				{
					ifTabu = true;
					break;
				}
			}
			//Kryterium aspiracji 
			if (((currentOptimalMin + b) >= optimalMin)&& (ifTabu)) 
			{
				continue;
			}
			
			if (b < bestB){
				
				bestB = b;
				bestI = i;
				bestJ = j;
			}	
		}
	}
	return bestB;
}

int LocalSearch::bestSwapNeighbor(int& bestI, int& bestJ, vector <unsigned> currentRoute) {

	bool ifTabu;
	int b, bestB = INT_MAX;
	bestI = 0;
	bestJ = 0;

	for (int i = 1; i < matrixSize - 1;i++) {	///
		for (int j = i + 1; j < matrixSize;j++) {///
			b = swapCalculator(i, j, currentRoute);
			ifTabu = false;
			for (int k = 0; k < tabuList.size();k++) {
				vector <unsigned> tempTabu = tabuList.at(k);
				if ((tempTabu.at(1) == currentRoute.at(i)) && (tempTabu.at(2) == currentRoute.at(j)))
				{
					ifTabu = true;
					break;
				}
				if ((tempTabu.at(1) == currentRoute.at(j)) && (tempTabu.at(2) == currentRoute.at(i)))
				{
					ifTabu = true;
					break;
				}
			}
			//Kryterium aspiracji

			if (((currentOptimalMin + b) >= optimalMin) && (ifTabu))
			{
				continue;
			}

			if (b < bestB) {

				bestB = b;
				bestI = i;
				bestJ = j;
			}

		}
	}
	return bestB;
}

int LocalSearch::bestInsertNeighbor(int& bestI, int& bestJ, vector <unsigned> currentRoute) {

	bool ifTabu;
	int b, bestB = INT_MAX;
	bestI = 0;
	bestJ = 0;

	for (int i = 1; i < matrixSize - 1;i++) {	///
		for (int j = 1; j < matrixSize;j++) {/// 
			if (i != j - 1 && i != j && i != j + 1) {
				b = insertCalculator(i, j, currentRoute);
				
				ifTabu = false;

				for (int k = 0; k < tabuList.size();k++) {
					if (tabuList.at(k).at(1) == currentRoute.at(i) && tabuList.at(k).at(2) == currentRoute.at(j))
					{
						ifTabu = true;
						break;
					}
				}
				//Kryterium aspiracji
				if (ifTabu) {
					if (currentOptimalMin + b >= optimalMin) {
						continue;
					}
				}
				if (b < bestB) {
					if (ifTabu) wasAspirated = true;
					else wasAspirated = false;
					bestB = b;
					bestI = i;
					bestJ = j;
				}
			}
		}
	}
	return bestB;
}

void LocalSearch::correctTabuList() {

	for (int i = 0; i < tabuList.size(); i++) {
		tabuList.at(i).at(0)--;

		if (tabuList.at(i).at(0) == 0) {
			vector <vector <unsigned>> ::iterator it;
			it = tabuList.begin() + i;
			tabuList.erase(it);

			//tabuList.erase(tabuList.begin() + i);
			//i-- ;
		}
	}
}

bool LocalSearch::ifContinueT(Czas timer)
{	
	timer.czasStop();
	if (timer.czasWykonaniaSek() >= timeLimit) return false;
	else return true;
}

void LocalSearch::diversification(vector <unsigned>& currentRoute)
{
	//kilka losowych swapów
	random_device randomSrc;
	default_random_engine randomGenerator(randomSrc());
	uniform_int_distribution<> randNode(1, matrixSize - 1);
	int x, y;

	for (int i = 0;i < matrixSize / 10;i++) {
		do {
			x = randNode(randomGenerator);
			y = randNode(randomGenerator);
		} while (x == y);
		//cout << "(" << x << "," << y << ")|";
		vSwap(x, y, currentRoute);
	}

	/*cout << "===================" << endl;
	cout << "===================" << endl;
	cout << "===================" << endl;*/
	//cout << endl;

}

void LocalSearch::saveToFileTabu(string name) {
	ofstream plik;
	plik.open("wynikiTestyTabu.csv", std::ios_base::app);
	plik << timeLimit << ";" << iterLimit << ";"  << neighborType  <<";" << optimalMin << ";" << timerSOL.czasWykonaniaSek() << ";";
	for (int i = 0; i < valueForTime.size(); i++) {
		plik << valueForTime.at(i) << ";";
	}

	plik << " " << ";";

	for (int j = 0; j < routeTab.size();j++) {
		plik << routeTab.at(j) << ";";
	}

	plik << endl;
	plik.close();
}

void LocalSearch::saveToFileSA(string name) {
	ofstream plik;
	plik.open("wynikiTestySA.csv", std::ios_base::app);
	plik << matrixSize << ";"<< cooling <<";" <<temp <<";" << best << ";" << timerSOL.czasWykonaniaSek() << ";";
	for (int i = 0; i < valueForTime.size(); i++) {
		plik << valueForTime.at(i) << ";";
	}

	plik << " " << ";";

	for (int j = 0; j < routeTab.size();j++) {
		plik << routeTab.at(j) << ";";
	}

	plik << endl;
	plik.close();
}