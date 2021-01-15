#pragma once

#include <iostream>
#include <vector>
#include "Czas.h"

using namespace std;

class Genetic
{
public:
	Genetic();
	void startGenetic();

	int** tspMatrix;
	int matrixSize;
	void setMatrix(int** tsp, int s);

	vector <unsigned> valueForTime;

	Czas timerGen;
	Czas timerSOL;

	int timeLimit;
	int popSize;
	double crossP;
	int crossType;
	double muteP;
	int generation;

	vector <unsigned> best;
	void saveToFileGenetic();
	int calcCost(vector<unsigned> v);
	void showRoute(vector <unsigned> v);
	void setGenetic(int time, int pop, double cross, double mute ,int crossss);
	bool ifContinue(Czas timer);
	void sortVector(vector <vector<unsigned>>& vect);
	void vSwap(int a, int b, vector <unsigned>& currentRoute);
	int swapCalculator(int x, int y, vector <unsigned>currentRoute);


	void initPopulation(vector<vector <unsigned>>& population);
	vector <unsigned> tournamentSelection(vector <vector <unsigned>>& pop);
	void crossOver(vector <unsigned> p1, vector <unsigned> p2, vector <unsigned>& c1, vector <unsigned>& c2);
	void PMX(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2);
	void OX(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2);

	void mutation(vector <unsigned>& c);

	void nextPopulation(vector <vector<unsigned>>& population, vector <vector<unsigned>>popul);
};

