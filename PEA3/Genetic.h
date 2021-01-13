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


	Czas timerGen;
	int timeLimit;
	int popSize;
	double crossP;
	double muteP;

	int calcCost(vector<unsigned> v);
	void showRoute(vector <unsigned> v);
	void setGenetic(int time, int pop, double cross, double mute);
	bool ifContinue();
	

	void initPopulation(vector<vector <unsigned>> pop);
	vector <unsigned> tournamentSelection(vector <vector <unsigned>> pop);
	void crossOver(vector <unsigned> p1, vector <unsigned> p2, vector <unsigned> c1, vector <unsigned>c2);
	void PMX(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2);

	void mutation(vector <unsigned> c1);
};

