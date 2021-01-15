#pragma once
#include <iostream>
#include <vector>
#include "Czas.h"

using namespace std;

class LocalSearch
{
public:
	LocalSearch(); 

	Czas timerSOL;
	Czas timerSA;
	vector<int> valueForTime;

	int** tspMatrix;
	int matrixSize;
	int timeLimit;
	int optimalMin;

	double initAlpha;

	void setSA(double c, int d);	
	void setTS(int time, int neig);
	int calcCost(vector <unsigned> v);
	void showRoute(vector <unsigned> v);
	bool ifContinueT(Czas timer);
	void setMatrix(int** tsp, int s);
	int getInitGreedy();
	
	int swapCalculator(int x, int y, vector <unsigned>currentRoute);
	int insertCalculator(int x, int y, vector <unsigned> currentRoute);
	int reverseCalculator(int x, int y, vector <unsigned> currentRoute);

	void vSwap(int a, int b, vector <unsigned>& currentRoute);
	void vReverse(int a, int b, vector <unsigned>& currentRoute);
	void vInsert(int a, int b, vector <unsigned>& currentRoute);

	
	

	
	//int getRandGreedy();
	
	void startSA();
	
	double initTemp;
	double minTemp;
	double cooling;
	int iterations;
	double temp;

	int shufflePath(vector <unsigned>& shuffled);
	bool moveOrNo(int currentOptimalMin, int currentCost, double temp);
	double changeTemp(double temp);
	int best;
	

	void startTS();
	
	vector< vector<unsigned> > tabuList;
	int tabuCadence;
	int currentTabuCadence;
	int iterLimit;
	int intensivity;
	int neighborType;

	bool wasAspirated;

	int bestNeighbor(int& bestI, int& bestJ, vector <unsigned> currentRoute);
	
	void correctTabuList();
	void diversification(vector <unsigned>& currentRoute);

	

	
	int bestSwapNeighbor(int& bestI, int& bestJ, vector <unsigned> currentRoute);
	int bestInsertNeighbor(int& bestI, int& bestJ, vector <unsigned> currentRoute);
	int bestReverseNeighbor(int& bestI, int& bestJ, vector <unsigned> currentRoute);
	void calculateSwap(int i, int j, int& balance, vector <unsigned> currentRoute);
	void calculateInsert(int i, int j, int& balance, vector <unsigned> currentRoute);
	void calculateReverse(int i, int j, int& balance, vector <unsigned> currentRoute);

	int currentCost;
	int currentOptimalMin;

	vector <unsigned> routeTab;
	vector <unsigned> currentRoute;

	
	void saveToFileTabu(string name);
	void saveToFileSA(string name);
	


	
};

