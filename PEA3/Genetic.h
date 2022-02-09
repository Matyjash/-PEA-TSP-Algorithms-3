#pragma once
#include "Graph.h"
#include <vector>
class Genetic
{
private:

	Graph* graph;

	std::vector<int> bestPath;
	int bestCost;

	float mutationFactor, crossoverFactor;
	int mutationMethod, crossoverMethod;

	std::vector<int> fitness;

	//czas rozpoczêcia
	long long int startTime;

	long long int frequency;

	/// <summary>
	/// Wyliczanie kosztu przejœcia przez podan¹ œcie¿kê
	/// </summary>
	/// <param name="path">Œcie¿ka w postaci wektora liczb</param>
	/// <returns>Liczba ca³kowita reprezentujaca d³ugoœæ œcie¿ki</returns>
	int getPathCost(std::vector <int> path);

	/// <summary>
	/// Mutacja poprzez zamiaê loswych miast w œcie¿ce
	/// </summary>
	/// <param name="path">Scie¿ka, która ma zostaæ poddana mutacji</param>
	/// <returns>Zmutowana œcie¿ka</returns>
	std::vector<int> mutateBySwap(std::vector<int> path);

	void printPopulation(std::vector<std::vector<int>> population);
	void printPath(std::vector<int>);

	/// <summary>
	/// Mutacja poprzez inwersjê w obrêbie losowej czêœci œcie¿ki
	/// </summary>
	/// <param name="path">Œcie¿ka, która ma zostaæ poddana mutacji</param>
	/// <returns>Zmutowana œcie¿ka</returns>
	std::vector<int> mutateByInversion(std::vector<int> path);

	/// <summary>
	/// Próba mutacji, powodzenie zale¿ne od wspó³czynnika mutacji
	/// </summary>
	/// <param name="path">Scie¿ka, która ma zostaæ poddana mutacji</param>
	/// </summary>
	std::vector<int> tryToMutate(std::vector<int> path);

	/// <summary>
	/// Metoda tworz¹ca now¹ populacje wybieraj¹c najlepsze jednostki z jednej i drugiej grupy
	/// </summary>
	/// <param name="population">Stara populacja</param>
	/// <returns>Nowa populacja</returns>
	std::vector<std::vector<int>> generateNewPopulation(std::vector<std::vector<int>> population);

	/// <summary>
	/// Metoda Partially Mapped Crossover krzyzujaca dwoch rodzicow w celu powstania potomka
	/// </summary>
	/// <param name="parent1">Rodzic 1</param>
	/// <param name="parent2">Rodzic 2</param>
	/// <param name="crossoverFactor">Wspolczynnik krzyzowania</param>
	/// <param name="mutationFactor">Wspolczynnik mutacji</param>
	/// <param name="mutationMethod">Metoda mutacji</param>
	/// <returns></returns>
	std::pair<std::vector<int>, std::vector<int>> crossOverPMX(std::vector<int> parent1, std::vector<int> parent2);

	/// <summary>
	/// Metoda Order Crossover krzyzujaca dwoch rodzicow w celu powstania potomka
	/// </summary>
	/// <param name="parent1">Rodzic 1</param>
	/// <param name="parent2">Rodzic 2</param>
	/// <param name="crossoverFactor">Wspolczynnik krzyzowania</param>
	/// <param name="mutationFactor">Wspolczynnik mutacji</param>
	/// <param name="mutationMethod">Metoda mutacji</param>
	/// <returns></returns>
	std::pair<std::vector<int>, std::vector<int>> crossOverOX(std::vector<int> parent1, std::vector<int> parent2);

	/// <summary>
	/// Metoda sprawdzaj¹ca czy podany czas ju¿ up³yn¹³
	/// </summary>
	/// <returns>True je¿eli czas up³yn¹³, False je¿eli nie</returns>
	bool isTimeUp();

	/// <summary>
	/// Metoda zamieniaj¹ca 2 losowe miasta kolejnoœci¹
	/// </summary>
	/// <param name="path">Œcie¿ka w której ma zostaæ przeprowadzona zamiana</param>
	void swapTwoRandomCities(std::vector<int>* path);

	/// <summary>
	/// Wybranie spoœród populacji 2 rodziców metod¹ turnieju
	/// </summary>
	/// <param name="population">Populacja z któej maj¹ zostaæ wybrani rodzice</param>
	/// <returns>Popoulacja rodziców</returns>
	std::vector<std::vector<int>> selectParentsByTournament(std::vector<std::vector<int>> population);



	/// <summary>
	/// Metoda oblcizaj¹ca wartoœci przystosowania poszczególnych rozwizañ, zapisuje je w wektorze fitness nale¿¹cym do klasy
	/// </summary>
	/// <param name="population">Populacja</param>
	void calculateFitness(std::vector<std::vector<int>> population);

	long long int read_QPC();

	/// <summary>
	/// Generowanie pocz¹tkowej œcie¿ki
	/// </summary>
	/// <returns>Losowa œcie¿ka z³o¿ona z n miast</returns>
	std::vector <int> generateRandomPath();
public:

	/// <summary>
	/// G³ówna metoda zawieraj¹ca algorytm genetyczny. Oblicza i pokazuje najlepsz¹ uzsykan¹ œcie¿kê w problemie TSP.
	/// </summary>
	/// <param name="graph">Graf zawieraj¹cy miasta</param>
	/// <param name="populationSize">Rozmiar populacji</param>
	/// <param name="mutationFactor">Wpspolczynnik mutacji</param>
	/// <param name="crossoverFactor">Wspolczynnik krzyzowania</param>
	/// <param name="mutationMethod">Metoda mutacji</param>
	/// <param name="crossoverMethod">Metoda krzyzowania</param>
	/// <param name="timeToStop">Czas trwania algorytmu</param>
	void findPath(Graph* graph, int populationSize, float mutationFactor, float crossoverFactor, int mutationMethod, int crossoverMethod, int timeToStop);
};

