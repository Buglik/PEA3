#include "Genetic.h"
#include <iostream>
#include <random>
#include <ctime>
#include <chrono>
#include <fstream>
#include "Czas.h"

using namespace std;

Genetic::Genetic(){}

void Genetic::setMatrix(int** tsp, int s) {
	tspMatrix = tsp;
	matrixSize = s;
}

void Genetic::setGenetic(int time, int pop, double cross, double mute){
	timeLimit = time;
	popSize = pop;
	crossP = cross;
	muteP = mute;
}

int Genetic::calcCost(vector<unsigned> v)
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

void Genetic::showRoute(vector <unsigned> v)
{
	for (int i = 0; i < v.size() - 1; i++)
		cout << v.at(i) << "->";
	cout << v.at(v.size() - 1);
	cout << endl;
}

void Genetic::startGenetic()
{
	vector < vector <unsigned>> population;
	vector <unsigned> parent1, parent2;
	vector <unsigned> child1(matrixSize + 2, 0), child2(matrixSize + 2, 0);
	vector <double> fitness(popSize, 0);

	timerGen.czasStart();
	initPopulation(population);	//TODO

	while (ifContinue())
	{
		vector<vector<unsigned>> newPopulation;

		while (newPopulation.size() < popSize)
		{
			parent1 = tournamentSelection(population);
			parent2 = tournamentSelection(population);

			crossOver(parent1, parent2, child1, child2);	// TODO 
			mutation(child1);	//TODO
			mutation(child2);	//TODO

			newPopulation.push_back(child1);
			newPopulation.push_back(child2);
		}
		//TODO polaczenie starej i nowej populacji i wpisanie do populacji

	}
	// TODO poka najlepszego z populacji ostatniej
}

bool Genetic::ifContinue()
{
	timerGen.czasStop();
	if (timerGen.czasWykonaniaSek() < timeLimit) return true;
	else return false;
}

void Genetic::crossOver(vector <unsigned> p1, vector <unsigned> p2, vector <unsigned> c1, vector <unsigned>c2)
{

}

void Genetic::mutation(vector <unsigned> c)
{

}

void Genetic::initPopulation(vector<vector <unsigned>> pop)
{

}

vector <unsigned> Genetic::tournamentSelection(vector <vector <unsigned>> pop) {
	vector <unsigned> best;
	vector <unsigned> ind;

	random_device randomSrc;
	default_random_engine randomGen(randomSrc());
	uniform_int_distribution<> indRand(0, pop.size() - 1);

	int k = 2;
	for (int i = 1; i <= k; i++) {
		if (i == 1)
			best = pop.at(indRand(randomGen));

		else {
			ind = pop.at(indRand(randomGen));
			if (ind.at(matrixSize + 1) < best.at(matrixSize + 1))
				best = ind;
		}
	}

	return best;
}