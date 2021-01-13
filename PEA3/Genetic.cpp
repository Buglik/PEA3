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
	PMX(p1, p2, c1, c2);
}

void Genetic::PMX(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2) {
	random_device randomSrc;
	default_random_engine randomGen(randomSrc());
	uniform_int_distribution<> nodeRand(1, matrixSize - 1);

	vector <unsigned> visitedOffspring1(matrixSize, 0);
	vector <unsigned> visitedOffspring2(matrixSize, 0);

	int a, b, balance = 0;

	do {
		a = nodeRand(randomGen);
		b = nodeRand(randomGen);
	} while (a == b || a > b);

	//cout << "A: " << a << endl;
	//cout << "B: " << b << endl;

	// SKOPIOWANIE SEGMENTU Z RODZICA
	for (int i = a; i < b; i++) {
		offspring1.at(i) = parent2.at(i);
		offspring2.at(i) = parent1.at(i);
		visitedOffspring1.at(parent2.at(i)) = 1;
		visitedOffspring2.at(parent1.at(i)) = 1;
	}

	// Przepisanie "czego sie da" przed sekwencja
	for (int i = a - 1; i >= 0; i--) {
		if (visitedOffspring1.at(parent1.at(i)) != 1) {
			offspring1.at(i) = parent1.at(i);
			if (parent1.at(i) != 0)
				visitedOffspring1.at(parent1.at(i)) = 1;
		}
		else
			offspring1.at(i) = 2021;

		if (visitedOffspring2.at(parent2.at(i)) != 1) {
			offspring2.at(i) = parent2.at(i);
			if (parent2.at(i) != 0)
				visitedOffspring2.at(parent2.at(i)) = 1;
		}
		else
			offspring2.at(i) = 2021;
	}

	// Przepisanie "czego sie da" po sekwencja
	for (int i = b; i <= matrixSize; i++) {
		if (visitedOffspring1.at(parent1.at(i)) != 1) {
			offspring1.at(i) = parent1.at(i);
			visitedOffspring1.at(parent1.at(i)) = 1;
		}
		else
			offspring1.at(i) = 2021;

		if (visitedOffspring2.at(parent2.at(i)) != 1) {
			offspring2.at(i) = parent2.at(i);
			visitedOffspring2.at(parent2.at(i)) = 1;
		}
		else
			offspring2.at(i) = 2021;
	}

	int help = 0;
	bool cont = true;

	for (int i = 0; i < matrixSize; i++) {
		if (offspring1.at(i) == 2021) {
			help = i;
			while (cont == true) {
				for (int j = 0; j < matrixSize; j++) {
					if (parent2.at(j) == parent1.at(help)) {
						if (visitedOffspring1.at(parent1.at(j)) != 1) {
							offspring1.at(i) = parent1.at(j);
							cont = false;
							break;
						}
						else {
							help = j;
						}
					}
				}
			}
			cont = true;
		}

		if (offspring2.at(i) == 2021) {
			help = i;
			while (cont == true) {
				for (int j = 0; j < matrixSize; j++) {
					if (parent1.at(j) == parent2.at(help)) {
						if (visitedOffspring2.at(parent2.at(j)) != 1) {
							offspring2.at(i) = parent2.at(j);
							cont = false;
							break;
						}
						else {
							help = j;
						}
					}
				}
			}
			cont = true;
		}
	}
	offspring1.at(matrixSize + 1) = calcCost(offspring1);
	offspring2.at(matrixSize + 1) = calcCost(offspring2);
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