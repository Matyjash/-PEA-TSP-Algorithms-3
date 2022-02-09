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

	//generujemy dwa ró¿ne od siebie indexy i dokonujemy zamiany wartosci kryj¹cych siê pod nimi
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

	//tworzymy liste zawieraj¹c¹ numery wszystkich miast (oprócz 0)
	for (int i = 1; i < graph->getSize(); i++) {
		path[i - 1] = i;
	}
	//'tasujemy' wektor
	std::random_shuffle(path.begin(), path.end());

	return path;
}

std::vector<std::vector<int>> Genetic::selectParentsByTournament(std::vector<std::vector<int>> population) {
	//populacja rodziców
	std::vector<std::vector<int>> parents;
	parents.resize(2);
	//wyznaczamy wielkoœæ turnieju czyli liczbe osobników bior¹cych w nim udzia³
	int tournamentSize = (int)(population.size()/8);
	if (tournamentSize == 0 || tournamentSize == 1) {
		tournamentSize = 2;

	}

	for (int i = 0; i < 2; i++) {
		int bestCost = INT_MAX;
		std::vector<int> bestPath;
		//odszukujemy najlepsz¹ drogê spoœród wszystkich wybranych losowo w turnieju
		for (int j = 0; j < tournamentSize; j++) {
			int randomIndex = Random::generateRandomNumber(0, population.size());
			int cost = this->fitness[randomIndex];
			if ( cost < bestCost) {
				bestCost = cost;
				bestPath = population[randomIndex];
			}
		}
		//dodajemy odnalezion¹ drogê do rodziców
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
	//je¿eli nie dojdzie do krzy¿owania to zwracamy rodziców
	if (random > crossoverFactor) {
		return std::make_pair(parent1, parent2);
	}
	else {
		//je¿eli dojdzie do krzy¿owania
		//wyznaczamy rodziców i punkty przeciêcia które pozwola przypisaæ czeœæ genów z pierwszego i czêœæ genów drugiego rodzica do potomka
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

		//Tworzymy wektor pomocniczy aby umieszaj¹c kolejne geny nie dosz³o do powtórki
		std::vector<bool> childNodes, childNodes2;
		childNodes.resize(graph->getSize());
		childNodes2.resize(graph->getSize());

		std::fill(childNodes.begin(), childNodes.end(), false);
		std::fill(childNodes2.begin(), childNodes2.end(), false);

		//wype³niamy miejsca miêdzy punktami przeciêcia i aktualizujemy wektory childNodes i childNodes2
		for (int j = crossPoint; j < crossPoint2; j++) {
			child[j] = parent1[j];
			childNodes[parent1[j]] = true;

			child2[j] = parent2[j];
			childNodes2[parent2[j]] = true;
		}

		int crossGapSize = crossPoint2 - crossPoint;
		//tworzymy wektory sk³adaj¹ce siê z ciagu genów w kolejnoœci od drugiego punktu przeciêcia do pierwszego punktu przeciêcia (przechodz¹c przez pocz¹tek)
		//pomijamy w nich jednak te wartoœci które zosta³y ju¿ wczeœniej przepisane
		std::vector<int> sequence1, sequence2;
		sequence1.resize(child.size() - crossGapSize);
		sequence2.resize(child.size() - crossGapSize);

		//najpierw przepisujemy czêœc od drugiego punktu przeciêcia do koñca
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

		//natêpnie przepisujemy czêœæ od pocz¹tku do drugiego punktu przeciêcia
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
		//teraz przepisujemy sekwencje do wektorów potomków ponownie zaczynaj¹c od drugiego punktu przeciêcia do koñca
		for (int i = crossPoint2; i < child.size(); i++) {
			child[i] = sequence1[index1]; index1++;
			child2[i] = sequence2[index2]; index2++;
		}
		//nastepnie przepisujemy czêœc od pocz¹tku do pierwszego punktu przeciêcia potomka
		for (int i = 0; i < crossPoint; i++) {
			child[i] = sequence1[index1]; index1++;
			child2[i] = sequence2[index2]; index2++;
		}

		//Próbujemy przeprowadziæ proces mutacji
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
	//je¿eli nie dojdzie do krzy¿owania to zwracamy rodziców
	if (random > crossoverFactor ) {
		return std::make_pair(parent1, parent2);
	}

	else {
		int* mapping1 = new int[graph->getSize()];
		int* mapping2 = new int[graph->getSize()];
		//je¿eli dojdzie do krzy¿owania
		//wyznaczamy rodziców i punkty przeciêcia które pozwola przypisaæ czeœæ genów z pierwszego i czêœæ genów drugiego rodzica do potomka
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
		//Tworzymy wektor pomocniczy aby umieszaj¹c kolejne geny nie dosz³o do powtórki
		std::vector<bool> childNodes, childNodes2;
		childNodes.resize(graph->getSize());
		childNodes2.resize(graph->getSize());

		std::fill(childNodes.begin(), childNodes.end(), false);
		std::fill(childNodes2.begin(), childNodes2.end(), false);

		//wype³niamy miejsca miêdzy punktami przeciêcia i wype³niamy tablcie mapowania
		for (int j = crossPoint; j < crossPoint2; j++) {
			child[j] = parent2[j];
			childNodes[parent2[j]] = true;
			mapping1[parent2[j]] = parent1[j];

			child2[j] = parent1[j];
			childNodes2[parent1[j]] = true;
			mapping2[parent1[j]] = parent2[j];
		}
		//Wype³niamy miejsce przed pierwszym punktem przeciêcia wartoœciami które nie bêd¹ powtórzeniem
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
		//wype³niamy miejsce po drugim punkcie przeciêcia
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
		//Próbujemy przeprowadziæ proces mutacji
			child = tryToMutate(child);
			child2 = tryToMutate(child2);
		}
	
	return std::make_pair(child,child2);

}

int Genetic::getPathCost(std::vector <int> path) {
	int cost = 0;
	//dodajemy koszt przejscia z mista 0 do pierwszego w œcie¿ce
	cost += graph->getMatrix()[0][path[0]];
	//dodajemy koszty przejœæ w œcie¿ce
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

	//zachowujemy czêœæ najlepszych rozwi¹zañ z poprzedniej populacji
	sort(population.begin(), population.end(), [this](auto i, auto j)->bool {return getPathCost(i) < getPathCost(j); });
	int j = 0;

	for (; i < newPopulation.size() - 1; i++) {
		newPopulation[i] = population[j];
		j++;
	}
	//dodajemy losowe rozwi¹zanie aby przynajmniej w ma³ym stopniu unikn¹æ stagnacji
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
	//funkcje zwi¹zane z pomiarem czasu
	srand(time(NULL));
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	Genetic::startTime = read_QPC();

	//przygotowujemy populacje pocz¹tkow¹
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