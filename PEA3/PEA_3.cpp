#include <iostream>
#include <windows.h>
#include <string>
#include <random>
#include <iomanip>
#include "Graph.h"
#include "Random.h"
#include "Genetic.h"

//kryterium stopu
int timeToStop = 10;
//wielkosc populacji
int populationSize = 50;
//wpolczynnik mutacji
float mutationFactor = 0.01;
//wspolczynnik krzyzowania
float crossoverFactor = 0.8;
//metoda mutacji
int mutationMethod = 0;
//metoda krzyzowania
int crossoverMethod = 0;

void makeGraphRandom(Graph* graph);

/// <summary>
/// Ustawianie nazwy pliku
/// </summary>
/// <returns>Wprowadzona nazwa</returns>
std::string setFileName() {
    system("CLS");
    std::string fileName;
    std::cout << "Podaj nazwe pliku do zaladowania wraz z jego rozszerzeniem:";
    std::cin >> fileName;
    return fileName;
}

bool isChoiceValid(int choice, int n1, int n2) {
    if (choice >= n1 && choice <= n2) return true;
    else return false;
}

void menu() {

    int choice = 0;
    std::string fileName;
    Graph graph(0, 0, 0);
    //TODO: sprawdzenie czy struktury nie sa puste przed wykonaniem na nich algorytmu
    while (choice != 11) {
        system("CLS");
        std::cout << "Kryterium stopu: " << timeToStop << " s\n";
        std::cout << "Wielkosc populacji: " << populationSize << "\n";
        std::cout << "Wpolcznnik mutacji: " << mutationFactor << "\n";
        std::cout << "Wpolczynnik krzyzowania: " << crossoverFactor << "\n";
        std::cout << "\n\n\n";
        std::cout << "MENU\n";
        std::cout << "Wybierz opcje:\n";
        std::cout << "1.Wczytaj graf miast z pliku\n";
        std::cout << "2.Wyswietl graf miast\n";
        std::cout << "3.Wprowadz kryterium stopu(w sekundach)\n";
        std::cout << "4.Wprowadz wielkosc populacji poczatkowej\n";
        std::cout << "5.Wprowadz wspolczynnik mutacji\n";
        std::cout << "6.Wprowadz wspolczynnik krzyzowania\n";
        std::cout << "7.Zmien metode mutacji\n";
        std::cout << "8.Zmien metode krzyzowania\n";
        std::cout << "9.Uruchom algorytm\n";
        std::cout << "10.Losuj graf\n";
        std::cout << "11.Wyjscie\n";
        std::cout << "Wybor:";
        std::cin >> choice;
        std::cin.clear();							//resetujemy status strumienia wejść
        if (isChoiceValid(choice, 1, 11)) {
            switch (choice) {
            case 1: {
                fileName = setFileName();
                graph.loadGraphFromFile(fileName);
                std::cout << "\nGraf zostal zaladowany!\n";
                system("PAUSE");
                break;
            }
            case 2: {
                graph.printGraphMatrix();
                system("PAUSE");
                break;
            }
            case 3: {
                system("CLS");
                std::cout << "Podaj czas w sekundach:";
                std::cin >> timeToStop;
                if (!std::cin.good()) {
                    std::cin.clear();
                    std::cin.ignore(256, '\n');
                    timeToStop = 10;
                    std::cout << "\nWprowadzono niedozwolona wartosc!\n";
                }
                else {
                    std::cout << "\nKtyterium stopu to teraz " << timeToStop << " s.\n";
                }

                system("PAUSE");
                break;
            }
            case 4: {
                system("CLS");
                std::cout << "Podaj wielkosc populacji:";
                std::cin >> populationSize;
                if (!std::cin.good()) {
                    std::cin.clear();
                    std::cin.ignore(256, '\n');
                    populationSize = 50;
                    std::cout << "\nWprowadzono niedozwolona wartosc!\n";
                }
                else {
                    std::cout << "\nRozmiar populacji to teraz " << populationSize << "\n";
                }
                system("PAUSE");
                break;
            }

            case 5: {
                system("CLS");
                std::cout << "Podaj wspolczynnik w postacji ulamka:";
                std::cin >> mutationFactor;
                if (!std::cin.good()) {
                    std::cin.clear();
                    std::cin.ignore(256, '\n');
                    mutationFactor = 0.01;
                    std::cout << "\nWprowadzono niedozwolona wartosc!\n";
                }
                else {
                    std::cout << "\nWspolczynnik mutacji wynosi teraz " << mutationFactor << "\n";
                }
                system("PAUSE");
                break;
            }
            case 6: {
                system("CLS");
                std::cout << "Podaj wspolczynnik w postaci ulamka:";
                std::cin >> crossoverFactor;
                if (!std::cin.good()) {
                    std::cin.clear();
                    std::cin.ignore(256, '\n');
                    crossoverFactor = 0.8;
                    std::cout << "\nWprowadzono niedozwolona wartosc!\n";
                }
                else {
                    std::cout << "\nWspolczynnik krzyzowania to teraz " << crossoverFactor << " s.\n";
                }
                system("PAUSE");
                break;
            }
            case 7: {
                system("cls");
                if (mutationMethod == 0) {
                    mutationMethod = 1;
                    std::cout << "Metoda mutacji to teraz INWERSJA LOSOWEJ CZĘŚCI ROZWIĄZANIA\n";
                }
                else if (mutationMethod == 1) {
                    mutationMethod = 0;
                    std::cout << "Metoda mutacji to teraz ZAMIANA 2 LOSOWYCH WARTOŚCI\n";
                }
                system("PAUSE");
                break;
            }
            case 8: {
                system("cls");
                if (crossoverMethod == 0) {
                    crossoverMethod = 1;
                    std::cout << "Metoda krzyzowania to teraz PMX\n";
                }
                else if (crossoverMethod == 1) {
                    crossoverMethod = 0;
                    std::cout << "Metoda krzyzowania to teraz OX\n";
                }
                system("PAUSE");
                break;
            }

            case 9: {
                system("CLS");
                Genetic* genetic = new Genetic();
                genetic->findPath(&graph, populationSize, mutationFactor,crossoverFactor, mutationMethod, crossoverMethod, timeToStop);
                system("PAUSE");
                break;
            }

            case 10: {
                system("CLS");
                int sizeToResizeTo;
                std::cout << "Podaj rozmiar grafu do wylosowania: ";
                std::cin >> sizeToResizeTo;
                system("CLS");
                if (sizeToResizeTo > 1) {
                    graph.resizeMatrix(sizeToResizeTo);
                    makeGraphRandom(&graph);
                    std::cout << "Wylosowano graf!";
                    system("PAUSE");
                }
                break;
            }

            case 11: {

                break;
            }
            }
        }
        else {
            std::cout << "Podano liczba spoza zakresu! Podaj liczbe jeszcze raz!";
            system("PAUSE");
        }
    }
    return;
}



void makeGraphRandom(Graph* graph) {


    for (int i = 0; i < graph->getSize(); i++) {
        for (int j = 0; j < graph->getSize(); j++) {
            if (i != j)
                graph->addEdge(i, j, Random::generateRandomNumber(1, 10000));
            else
                graph->addEdge(i, j, 0);
        }
    }

    return;
}

#pragma optimize("", off)
int main()
{
    menu();
    /*
    int testGraphSize = 20;

    long float totalTime = 0;

    long long int frequency, start, elapsed;
    srand(time(NULL));
    QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

    for (int i = 0; i < 100; i++) {
        Graph* testGraph = new Graph(testGraphSize, 0, 0);
        makeGraphRandom(testGraph);

        start = read_QPC();//
        BB::findPath(testGraph);
        elapsed = read_QPC() - start;//

        std::cout << "Time [us] = " << (1000000.0 * elapsed) /
            frequency << "\n";
        totalTime += (1000000.0 * elapsed) /
            frequency;
    }
    std::cout << "Srednia:" << totalTime / 100000<< "[ms]\n";
    system("PAUSE");
    */

    return 0;

}
#pragma optimize("", on)
