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

	//czas rozpocz�cia
	long long int startTime;

	long long int frequency;

	/// <summary>
	/// Wyliczanie kosztu przej�cia przez podan� �cie�k�
	/// </summary>
	/// <param name="path">�cie�ka w postaci wektora liczb</param>
	/// <returns>Liczba ca�kowita reprezentujaca d�ugo�� �cie�ki</returns>
	int getPathCost(std::vector <int> path);

	/// <summary>
	/// Mutacja poprzez zamia� loswych miast w �cie�ce
	/// </summary>
	/// <param name="path">Scie�ka, kt�ra ma zosta� poddana mutacji</param>
	/// <returns>Zmutowana �cie�ka</returns>
	std::vector<int> mutateBySwap(std::vector<int> path);

	void printPopulation(std::vector<std::vector<int>> population);
	void printPath(std::vector<int>);

	/// <summary>
	/// Mutacja poprzez inwersj� w obr�bie losowej cz�ci �cie�ki
	/// </summary>
	/// <param name="path">�cie�ka, kt�ra ma zosta� poddana mutacji</param>
	/// <returns>Zmutowana �cie�ka</returns>
	std::vector<int> mutateByInversion(std::vector<int> path);

	/// <summary>
	/// Pr�ba mutacji, powodzenie zale�ne od wsp�czynnika mutacji
	/// </summary>
	/// <param name="path">Scie�ka, kt�ra ma zosta� poddana mutacji</param>
	/// </summary>
	std::vector<int> tryToMutate(std::vector<int> path);

	/// <summary>
	/// Metoda tworz�ca now� populacje wybieraj�c najlepsze jednostki z jednej i drugiej grupy
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
	/// Metoda sprawdzaj�ca czy podany czas ju� up�yn��
	/// </summary>
	/// <returns>True je�eli czas up�yn��, False je�eli nie</returns>
	bool isTimeUp();

	/// <summary>
	/// Metoda zamieniaj�ca 2 losowe miasta kolejno�ci�
	/// </summary>
	/// <param name="path">�cie�ka w kt�rej ma zosta� przeprowadzona zamiana</param>
	void swapTwoRandomCities(std::vector<int>* path);

	/// <summary>
	/// Wybranie spo�r�d populacji 2 rodzic�w metod� turnieju
	/// </summary>
	/// <param name="population">Populacja z kt�ej maj� zosta� wybrani rodzice</param>
	/// <returns>Popoulacja rodzic�w</returns>
	std::vector<std::vector<int>> selectParentsByTournament(std::vector<std::vector<int>> population);



	/// <summary>
	/// Metoda oblcizaj�ca warto�ci przystosowania poszczeg�lnych rozwiza�, zapisuje je w wektorze fitness nale��cym do klasy
	/// </summary>
	/// <param name="population">Populacja</param>
	void calculateFitness(std::vector<std::vector<int>> population);

	long long int read_QPC();

	/// <summary>
	/// Generowanie pocz�tkowej �cie�ki
	/// </summary>
	/// <returns>Losowa �cie�ka z�o�ona z n miast</returns>
	std::vector <int> generateRandomPath();
public:

	/// <summary>
	/// G��wna metoda zawieraj�ca algorytm genetyczny. Oblicza i pokazuje najlepsz� uzsykan� �cie�k� w problemie TSP.
	/// </summary>
	/// <param name="graph">Graf zawieraj�cy miasta</param>
	/// <param name="populationSize">Rozmiar populacji</param>
	/// <param name="mutationFactor">Wpspolczynnik mutacji</param>
	/// <param name="crossoverFactor">Wspolczynnik krzyzowania</param>
	/// <param name="mutationMethod">Metoda mutacji</param>
	/// <param name="crossoverMethod">Metoda krzyzowania</param>
	/// <param name="timeToStop">Czas trwania algorytmu</param>
	void findPath(Graph* graph, int populationSize, float mutationFactor, float crossoverFactor, int mutationMethod, int crossoverMethod, int timeToStop);
};

