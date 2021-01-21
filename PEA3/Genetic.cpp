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

void Genetic::setGenetic(int time, int pop, double cross, double mute, int crossss){
	timeLimit = time;
	popSize = pop;
	crossP = cross;
	muteP = mute;
	crossType = crossss;
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
	cout << "START" << endl;
	valueForTime.clear();
	vector < vector <unsigned>> population;
	vector <unsigned> parent1, parent2;
	vector <unsigned> child1(matrixSize + 3, 0), child2(matrixSize + 3, 0);
	Czas timerFive;

	timerGen.czasStart();
	timerFive.czasStart();
	timerSOL.czasStart();
	generation = 0;
	initPopulation(population);						//Utworzenie poczatkowej losowej generacji
	sortVector(population);							//Sortowanie 
	valueForTime.push_back(population.at(0).at(matrixSize + 1));
	best = population.at(0);
	
	do
	{
		vector<vector<unsigned>> newPopulation;
		vector<vector<unsigned>> availableParents = population;
		while (newPopulation.size() < popSize * crossP)
		{
			//Selekcja rodzicow
			parent1 = tournamentSelection(availableParents);
			parent2 = tournamentSelection(availableParents);

			crossOver(parent1, parent2, child1, child2);	//krzyzowanie
			mutation(child1);	//mutacja (opcjonalna)
			mutation(child2);	

			newPopulation.push_back(child1);	//dodanie do nowej populacji
			newPopulation.push_back(child2);
		}
		sortVector(newPopulation);				//posortownaie jej (do testow)
		nextPopulation(population, newPopulation);
		generation++;

		//pomiar do sprawozdania (blad w czasie)
		timerFive.czasStop();
		if (timerFive.czasWykonaniaMili() >= 500)
		{
			valueForTime.push_back(population.at(0).at(matrixSize + 1));
			timerFive.czasStart();
		}
		//zapisanie czasu znalezienia najlepszego
		if (population.at(0).at(matrixSize + 1) < best.at(matrixSize + 1))
		{
			best = population.at(0);
			timerSOL.czasStop();
		}

	} while (ifContinue(timerGen));

	//best = population.at(0);
	cout << "KONIEC" << endl;
	cout << "Najlepsza wartosc: " << best.at(matrixSize + 1) << endl;
	cout << "Pochodzi z: " << best.at(matrixSize + 2) << " generacji" << endl;
	cout << "Znaleziona zostala w: " << timerSOL.czasWykonaniaMili() << " ms." << endl;
	cout << "Przeszlo przez " << generation << " generacji" << endl;

}

void Genetic::nextPopulation(vector <vector<unsigned>>& population, vector <vector<unsigned>>newPop) {
	/*for (int i = elitNumber; i < popSize; i++)
		population.at(i) = newPop.at(i - elitNumber);*/
	for (int i = 0;i < newPop.size();i++)
	{
		population.push_back(newPop.at(i));				//Poalczenie nowych potomków i starych potomkow
	}
	sortVector(population);								// posegregowanie ich wzgledem wartosci sciezki
	population.resize(popSize);							// odciecie slabszych osobnikow do rozmiar populacji
}


void Genetic::sortVector(vector <vector<unsigned>>& vect) {
	std::sort(vect.begin(), vect.end(),
		[&](const std::vector<unsigned>& a, const std::vector<unsigned>& b) {
			return a.at(matrixSize + 1) < b.at(matrixSize + 1);
		});
}

bool Genetic::ifContinue(Czas timer)
{
	timer.czasStop();
	if (timer.czasWykonaniaSek() >= timeLimit) return false;
	else return true;
}

void Genetic::crossOver(vector <unsigned> p1, vector <unsigned> p2, vector <unsigned>& c1, vector <unsigned>& c2)
{
	if (crossType == 0) PMX(p1, p2, c1, c2);
	else if (crossType == 1) OX(p1, p2, c1, c2);
	else cout << "BlAD!!!!!!!!!!!!!!!!!!!!!";
}

void Genetic::OX(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2) {
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

	// Przepisanie sekcji dopasowania
	for (int i = a; i < b; i++) {
		offspring1.at(i) = parent1.at(i);
		offspring2.at(i) = parent2.at(i);
		visitedOffspring1.at(parent1.at(i)) = 1;
		visitedOffspring2.at(parent2.at(i)) = 1;
	}


	int omitted = 0, omitted2 = 0;


	//wpisanie od drugiego przeciecie do konca (przed zapetleniem)
	for (int i = b; i < matrixSize; i++) {
		if (visitedOffspring1.at(parent2.at(i)) != 1) {
			offspring1.at(i - omitted) = parent2.at(i);
			visitedOffspring1.at(parent2.at(i)) = 1;
		}
		else
			omitted++;

		if (visitedOffspring2.at(parent1.at(i)) != 1) {
			offspring2.at(i - omitted2) = parent1.at(i);
			visitedOffspring2.at(parent1.at(i)) = 1;
		}
		else
			omitted2++;

	}

	int helpOmitted1 = omitted, helpOmitted2 = omitted2;


	//wpisanie od poczatku do pierwszego przeciecia (po zapetleniu)
	for (int i = 1; i < b; i++) {

		if (visitedOffspring1.at(parent2.at(i)) != 1) {
			if (omitted != 0) {
				offspring1.at(matrixSize - omitted) = parent2.at(i);
				visitedOffspring1.at(parent2.at(i)) = 1;
				omitted--;
			}

			else {
				offspring1.at(i - helpOmitted1) = parent2.at(i);
				visitedOffspring1.at(parent2.at(i)) = 1;
			}
		}

		else {
			helpOmitted1++;
		}


		if (visitedOffspring2.at(parent1.at(i)) != 1) {
			if (omitted2 != 0) {
				offspring2.at(matrixSize - omitted2) = parent1.at(i);
				visitedOffspring2.at(parent1.at(i)) = 1;
				omitted2--;
			}

			else {
				offspring2.at(i - helpOmitted2) = parent1.at(i);
				visitedOffspring2.at(parent1.at(i)) = 1;
			}
		}

		else {
			helpOmitted2++;
		}

	}

	offspring1.at(matrixSize + 1) = calcCost(offspring1);
	offspring2.at(matrixSize + 1) = calcCost(offspring2);

	offspring2.at(matrixSize + 2) = generation;
	offspring1.at(matrixSize + 2) = generation;
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

	offspring1.at(matrixSize + 2) = generation;
	offspring2.at(matrixSize + 2) = generation;
}


void Genetic::mutation(vector <unsigned>& c)
{
	if (static_cast<float>(rand()) / RAND_MAX < muteP) {		//mutacja opcjonalna
		int balance, i, j;
		random_device randomSrc;
		default_random_engine randomGen(randomSrc());
		uniform_int_distribution<> nodeRand(1, matrixSize - 1);

		do {
			i = nodeRand(randomGen);
			j = nodeRand(randomGen);
		} while (i == j || j<i);
		
		balance = swapCalculator(i, j, c);
		vSwap(i, j, c);
		c.at(matrixSize + 1) += balance;
	}
}

int Genetic::swapCalculator(int x, int y, vector <unsigned>currentRoute)
{
	int bal;
	vector <unsigned> tempR = currentRoute;
	vSwap(x, y, tempR);
	bal = calcCost(tempR) - calcCost(currentRoute);
	return bal;
}
void Genetic::vSwap(int a, int b, vector <unsigned>& currentRoute) {
	unsigned buffer = currentRoute.at(b);
	currentRoute.at(b) = currentRoute.at(a);
	currentRoute.at(a) = buffer;
}

void Genetic::initPopulation(vector<vector <unsigned>>& population)
{
	int n;
	int cost;
	for (int i = 0;i < popSize;i++)
	{
		vector <unsigned> tempV;
		vector <bool> visited(matrixSize, false);
		
		tempV.push_back(0);
		visited.at(0) = true;

		random_device randomSrc;
		default_random_engine randomGen(randomSrc());
		uniform_int_distribution<> nodeRand(1, matrixSize - 1);
		while (tempV.size() != matrixSize)
		{
			n = nodeRand(randomGen);
			if (visited.at(n) == false)
			{
				tempV.push_back(n);
				visited.at(n) = true;
			}
		}
		tempV.push_back(0);
		cost = calcCost(tempV);
		tempV.push_back(cost);
		tempV.push_back(generation);

		population.push_back(tempV);
	}
}

vector <unsigned> Genetic::tournamentSelection(vector <vector <unsigned>>& parents) {
	vector <unsigned> best;
	vector <unsigned> ind;

	random_device randomSrc;
	default_random_engine randomGen(randomSrc());
	uniform_int_distribution<> indRand(0, parents.size() - 1);
	int helper,helper2;

			helper = indRand(randomGen);		//losuje pierwszego rodzica
			best = parents.at(helper);

			helper2 = indRand(randomGen);		//losuje drugiego
			ind = parents.at(helper2);
			if (ind.at(matrixSize + 1) < best.at(matrixSize + 1))	//i ich porownuje
			{
				best = ind;
				parents.erase(parents.begin() + helper2);
			}
			else
			{
				parents.erase(parents.begin() + helper);
			}
	return best;
}

void Genetic::saveToFileGenetic() {
	ofstream plik;
	plik.open("wynikiGenetic.csv", std::ios_base::app);
	plik <<matrixSize <<";"<< timeLimit <<";"<< crossType << ";" << popSize << ";" << crossP << ";" << muteP << ";" << best.at(matrixSize + 1) << ";" << best.at(matrixSize + 2) << ";"<<generation<<";" << timerSOL.czasWykonaniaMili() << ";";
	plik << " " << ";";

	for (int i = 0; i < valueForTime.size(); i++) {
		plik << valueForTime.at(i) << ";";
	}
	plik << " " << ";";

	plik << endl;
	plik.close();
}