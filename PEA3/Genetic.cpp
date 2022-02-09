#include "Genetic.h"
#include <iostream>
#include <windows.h>
#include <time.h>
#include "Random.h"
#include <random>
#include <queue>
void Genetic::calculateFitness(std::vector<std::vector<int>> population) {
	for (int i = 0; i < population.size(); i++) {
		this->fitness[i] = getPathCost(population[i]);
		if (this->fitness[i] < this->bestCost) {
			this->bestPath = population[i];
			this->bestCost = this->fitness[i];
		}
	}
}
void Genetic::printPopulation(std::vector<std::vector<int>> population) {
	for (int i = 0; i < population.size(); i++) {
		printPath(population[i]);
	}
	std::cout << "--------------------------------------------------";
}
void Genetic::printPath(std::vector<int> path) {
	std::cout << "\n";
	std::cout << "0->";
	for (int i = 0; i < path.size(); i++) {
		std::cout << path[i] << "->";
	}
	std::cout << "0  ";
	std::cout << "Cost: " << getPathCost(path) << "\n";

}

long long int Genetic::read_QPC()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	return((long long int)count.QuadPart);
}

void Genetic::swapTwoRandomCities(std::vector<int>* path) {

	//generujemy dwa r�ne od siebie indexy i dokonujemy zamiany wartosci kryj�cych si� pod nimi
	int city1Index = Random::generateRandomNumber(0, graph->getSize() - 1);
	int city2Index = Random::generateRandomNumber(0, graph->getSize() - 1);

	while (city1Index == city2Index) {
		city2Index = Random::generateRandomNumber(0, graph->getSize() - 1);
	}

	std::swap((*path)[city1Index], (*path)[city2Index]);
}

std::vector<int> Genetic::generateRandomPath() {
	std::vector<int> path;
	path.resize(graph->getSize() - 1);

	//tworzymy liste zawieraj�c� numery wszystkich miast (opr�cz 0)
	for (int i = 1; i < graph->getSize(); i++) {
		path[i - 1] = i;
	}
	//'tasujemy' wektor
	std::random_shuffle(path.begin(), path.end());

	return path;
}

std::vector<std::vector<int>> Genetic::selectParentsByTournament(std::vector<std::vector<int>> population) {
	//populacja rodzic�w
	std::vector<std::vector<int>> parents;
	parents.resize(2);
	//wyznaczamy wielko�� turnieju czyli liczbe osobnik�w bior�cych w nim udzia�
	int tournamentSize = (int)(population.size()/8);
	if (tournamentSize == 0 || tournamentSize == 1) {
		tournamentSize = 2;

	}

	for (int i = 0; i < 2; i++) {
		int bestCost = INT_MAX;
		std::vector<int> bestPath;
		//odszukujemy najlepsz� drog� spo�r�d wszystkich wybranych losowo w turnieju
		for (int j = 0; j < tournamentSize; j++) {
			int randomIndex = Random::generateRandomNumber(0, population.size());
			int cost = this->fitness[randomIndex];
			if ( cost < bestCost) {
				bestCost = cost;
				bestPath = population[randomIndex];
			}
		}
		//dodajemy odnalezion� drog� do rodzic�w
		parents[i] = bestPath;
	}

	return parents;
}

std::vector<int> Genetic::tryToMutate(std::vector<int> path) {
	float random = (float)(Random::generateRandomNumber(0, 1000) / 1000);
	if (random < mutationFactor) {
		if (mutationMethod == 0) {
			return mutateBySwap(path);
		}
		if (mutationMethod == 1) {
			return mutateByInversion(path);
		}
	}
	else {
		return path;
	}
}

std::vector<int> Genetic::mutateBySwap(std::vector<int> path) {
		swapTwoRandomCities(&path);
		return path;
}

std::vector<int> Genetic::mutateByInversion(std::vector<int> path) {

	int inversePoint = Random::generateRandomNumber(1, graph->getSize() - 1);
	int inversePoint2 = Random::generateRandomNumber(1, graph->getSize() - 1);
	while (inversePoint == inversePoint2) {
		inversePoint2 = Random::generateRandomNumber(1, graph->getSize() - 1);
	}
	if (inversePoint > inversePoint2) {
		int temp = inversePoint2;
		inversePoint2 = inversePoint;
		inversePoint = temp;
	}

	std::reverse(path.begin()+inversePoint, path.begin()+inversePoint2);

	return path;
}

std::pair<std::vector<int>, std::vector<int>> Genetic::crossOverOX(std::vector<int> parent1, std::vector<int> parent2) {
	//potomek
	std::vector<int> child;
	std::vector<int> child2;

	child.resize(parent1.size());
	child2.resize(parent2.size());

	float random = (float)(Random::generateRandomNumber(1, 1000) / 1000);
	//je�eli nie dojdzie do krzy�owania to zwracamy rodzic�w
	if (random > crossoverFactor) {
		return std::make_pair(parent1, parent2);
	}
	else {
		//je�eli dojdzie do krzy�owania
		//wyznaczamy rodzic�w i punkty przeci�cia kt�re pozwola przypisa� cze�� gen�w z pierwszego i cz�� gen�w drugiego rodzica do potomka
		int crossPoint = Random::generateRandomNumber(1, child.size());
		int crossPoint2 = Random::generateRandomNumber(1, child.size());
		while (crossPoint == crossPoint2) {
			crossPoint2 = Random::generateRandomNumber(1, child.size());
		}
		if (crossPoint > crossPoint2) {
			int temp = crossPoint2;
			crossPoint2 = crossPoint;
			crossPoint = temp;
		}

		//Tworzymy wektor pomocniczy aby umieszaj�c kolejne geny nie dosz�o do powt�rki
		std::vector<bool> childNodes, childNodes2;
		childNodes.resize(graph->getSize());
		childNodes2.resize(graph->getSize());

		std::fill(childNodes.begin(), childNodes.end(), false);
		std::fill(childNodes2.begin(), childNodes2.end(), false);

		//wype�niamy miejsca mi�dzy punktami przeci�cia i aktualizujemy wektory childNodes i childNodes2
		for (int j = crossPoint; j < crossPoint2; j++) {
			child[j] = parent1[j];
			childNodes[parent1[j]] = true;

			child2[j] = parent2[j];
			childNodes2[parent2[j]] = true;
		}

		int crossGapSize = crossPoint2 - crossPoint;
		//tworzymy wektory sk�adaj�ce si� z ciagu gen�w w kolejno�ci od drugiego punktu przeci�cia do pierwszego punktu przeci�cia (przechodz�c przez pocz�tek)
		//pomijamy w nich jednak te warto�ci kt�re zosta�y ju� wcze�niej przepisane
		std::vector<int> sequence1, sequence2;
		sequence1.resize(child.size() - crossGapSize);
		sequence2.resize(child.size() - crossGapSize);

		//najpierw przepisujemy cz�c od drugiego punktu przeci�cia do ko�ca
		int index1 = 0, index2 =0;
		for (int i = crossPoint2; i < child.size(); i++) {
			if (childNodes[parent2[i]] == false) {
				sequence1[index1] = parent2[i];
				index1++;
			}
			if (childNodes2[parent1[i]] == false) {
				sequence2[index2] = parent1[i];
				index2++;
			}
			
		}

		//nat�pnie przepisujemy cz�� od pocz�tku do drugiego punktu przeci�cia
		for (int i = 0; i < crossPoint2; i++) {
			if (childNodes[parent2[i]] == false) {
				sequence1[index1] = parent2[i];
				index1++;
			}
			if (childNodes2[parent1[i]] == false) {
				sequence2[index2] = parent1[i];
				index2++;
			}
		}

		index1 = 0; index2 = 0;
		//teraz przepisujemy sekwencje do wektor�w potomk�w ponownie zaczynaj�c od drugiego punktu przeci�cia do ko�ca
		for (int i = crossPoint2; i < child.size(); i++) {
			child[i] = sequence1[index1]; index1++;
			child2[i] = sequence2[index2]; index2++;
		}
		//nastepnie przepisujemy cz�c od pocz�tku do pierwszego punktu przeci�cia potomka
		for (int i = 0; i < crossPoint; i++) {
			child[i] = sequence1[index1]; index1++;
			child2[i] = sequence2[index2]; index2++;
		}

		//Pr�bujemy przeprowadzi� proces mutacji
		child = tryToMutate(child);
		child2 = tryToMutate(child2);
	}
	return std::make_pair(child, child2);
}

std::pair<std::vector<int>, std::vector<int>> Genetic::crossOverPMX(std::vector<int> parent1, std::vector<int> parent2) {
	//potomek
	std::vector<int> child;
	std::vector<int> child2;

	child.resize(graph->getSize() - 1);
	child2.resize(graph->getSize() - 1);

	float random = (float)(Random::generateRandomNumber(1, 1000)/1000);
	//je�eli nie dojdzie do krzy�owania to zwracamy rodzic�w
	if (random > crossoverFactor ) {
		return std::make_pair(parent1, parent2);
	}

	else {
		int* mapping1 = new int[graph->getSize()];
		int* mapping2 = new int[graph->getSize()];
		//je�eli dojdzie do krzy�owania
		//wyznaczamy rodzic�w i punkty przeci�cia kt�re pozwola przypisa� cze�� gen�w z pierwszego i cz�� gen�w drugiego rodzica do potomka
		int crossPoint = Random::generateRandomNumber(1, graph->getSize() - 1);
		int crossPoint2 = Random::generateRandomNumber(1, graph->getSize() - 1);
		while (crossPoint == crossPoint2) {
				crossPoint2 = Random::generateRandomNumber(1, graph->getSize() - 1);
		}
		if (crossPoint > crossPoint2) {
			int temp = crossPoint2;
			crossPoint2 = crossPoint;
			crossPoint = temp;
		}
		//Tworzymy wektor pomocniczy aby umieszaj�c kolejne geny nie dosz�o do powt�rki
		std::vector<bool> childNodes, childNodes2;
		childNodes.resize(graph->getSize());
		childNodes2.resize(graph->getSize());

		std::fill(childNodes.begin(), childNodes.end(), false);
		std::fill(childNodes2.begin(), childNodes2.end(), false);

		//wype�niamy miejsca mi�dzy punktami przeci�cia i wype�niamy tablcie mapowania
		for (int j = crossPoint; j < crossPoint2; j++) {
			child[j] = parent2[j];
			childNodes[parent2[j]] = true;
			mapping1[parent2[j]] = parent1[j];

			child2[j] = parent1[j];
			childNodes2[parent1[j]] = true;
			mapping2[parent1[j]] = parent2[j];
		}
		//Wype�niamy miejsce przed pierwszym punktem przeci�cia warto�ciami kt�re nie b�d� powt�rzeniem
		for (int j = 0; j < crossPoint; j++) {
			if (childNodes[parent1[j]] != true) {
				child[j] = parent1[j];
				childNodes[parent1[j]] = true;
			}
			else {
				int v = mapping1[parent1[j]];
				while (childNodes[v] == true) {
					v = mapping1[v];
				}
				child[j] = v;
				childNodes[v] = true;
			}
			if (childNodes2[parent2[j]] != true) {
				child2[j] = parent2[j];
				childNodes2[parent2[j]] = true;
			}
			else {
				int v = mapping2[parent2[j]];
				while (childNodes2[v] == true) {
					v = mapping2[v];
				}
				child2[j] = v;
				childNodes2[v] = true;
			}
		}
		//!!!
		//wype�niamy miejsce po drugim punkcie przeci�cia
		for (int j = crossPoint2; j < child.size(); ++j) {
			if (childNodes[parent1[j]] != true) {
				child[j] = parent1[j];
				childNodes[parent1[j]] = true;
			}
			else {
				int v = mapping1[parent1[j]];
				while (childNodes[v] == true) {
					v = mapping1[v];
				}
				child[j] = v;
				childNodes[v] = true;
			}

			if (childNodes2[parent2[j]] != true) {
				child2[j] = parent2[j];
				childNodes2[parent2[j]] = true;
			}
			else {
				int v = mapping2[parent2[j]];
				while (childNodes2[v] == true) {
					v = mapping2[v];
				}
				child2[j] = v;
				childNodes2[v] = true;
			}
		}

		delete[]mapping1;
		delete[]mapping2;
		//Pr�bujemy przeprowadzi� proces mutacji
			child = tryToMutate(child);
			child2 = tryToMutate(child2);
		}
	
	return std::make_pair(child,child2);

}

int Genetic::getPathCost(std::vector <int> path) {
	int cost = 0;
	//dodajemy koszt przejscia z mista 0 do pierwszego w �cie�ce
	cost += graph->getMatrix()[0][path[0]];
	//dodajemy koszty przej�� w �cie�ce
	for (unsigned int i = 0; i < path.size() - 1; i++) {
		cost += graph->getMatrix()[path[i]][path[i + 1]];
	}
	//dodajemy koszt przejscia z miasta ostatniego do 0
	cost += graph->getMatrix()[path[path.size() - 1]][0];

	return cost;
}

std::vector<std::vector<int>> Genetic::generateNewPopulation(std::vector<std::vector<int>> population) {
	//nowa populacja
	std::vector<std::vector<int>> newPopulation;
	newPopulation.resize(population.size());

	int elitismSize = (int)(population.size() / 10);

	int i = 0;
	for (; i < newPopulation.size()-elitismSize-1; i+=2) {
		std::vector<std::vector<int>> parents = selectParentsByTournament(population);
		std::pair<std::vector<int>, std::vector<int>> children;
		if (crossoverMethod == 0) {
			 children = crossOverOX(parents[0], parents[1]);
		}
		else if(crossoverMethod ==1){
			children = crossOverPMX(parents[0], parents[1]);
		}
		newPopulation[i] = children.first;
		newPopulation[i + 1] = children.second;
	}

	//zachowujemy cz�� najlepszych rozwi�za� z poprzedniej populacji
	sort(population.begin(), population.end(), [this](auto i, auto j)->bool {return getPathCost(i) < getPathCost(j); });
	int j = 0;

	for (; i < newPopulation.size() - 1; i++) {
		newPopulation[i] = population[j];
		j++;
	}
	//dodajemy losowe rozwi�zanie aby przynajmniej w ma�ym stopniu unikn�� stagnacji
	newPopulation[newPopulation.size() - 1] = generateRandomPath();


	return newPopulation;
}

void Genetic::findPath(Graph* graph, int populationSize, float mutationFactor, float crossoverFactor, int mutationMethod, int crossoverMethod, int timeToStop) {
	this->fitness.resize(populationSize);
	this->mutationFactor = mutationFactor;
	this->crossoverFactor = crossoverFactor;
	this->mutationMethod = mutationMethod;

	long long int timeSpent = 0;
	//przypisujemy graf
	Genetic::graph = graph;
	//najlepszy znaleziony do tej pory koszt i najlepsza znaleziona do tej pory droga
	//funkcje zwi�zane z pomiarem czasu
	srand(time(NULL));
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	Genetic::startTime = read_QPC();

	//przygotowujemy populacje pocz�tkow�
	std::vector<std::vector<int>> population;
	population.resize(populationSize);
	for (int i = 0; i < populationSize; i++) {
		population[i] = generateRandomPath();
	}
	bestCost = getPathCost(population[0]);

	bestPath = population[0];

		while (timeSpent < timeToStop) {
		calculateFitness(population);
		population = generateNewPopulation(population);
		timeSpent = ((read_QPC() - startTime) / frequency);

	}

	std::cout << "Koniec czasu przeszukiwania rozwiazan!\n";
	std::cout << "Czas: " << timeSpent << "[s]\n";
	std::cout << "Najlepsza odnaleziona sciezka: ";
	printPath(bestPath);
	return;
}