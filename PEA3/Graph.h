#pragma once
#include <string>
class Graph
{
private:
	int** matrix;		// tablica dwuwymiarowa jako macierz grafu
	int size;
	int startNode;
	int endNode;
	int edges;

public:
	Graph(int size, int startNode, int endNode);

	/// <summary>
	/// Dodawanie kraw�dzi o podanych wsp�rz�dnych i wadze
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="weight"></param>
	/// <returns></returns>
	void addEdge(int x, int y, int weight);

	/// <summary>
	/// Usuwanie kraw�dzi o podanych wsp�rz�dnych
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns></returns>
	void deleteEdge(int x, int y);

	/// <summary>
	/// Wy�wietlnie macierzy w konsoli
	/// </summary>
	void printGraphMatrix();

	int getSize();

	int getStartNode();

	int getEndNode();

	int** getMatrix();

	/// <summary>
	/// Zmiana rozmiaru macierzy
	/// </summary>
	/// <param name="newSize"></param>
	void resizeMatrix(int newSize);

	/// <summary>
	/// Wype�nienie macierzy okre�lonymi warto�ciami
	/// </summary>
	/// <param name="value"></param>
	void fillMatrix(int** matrix, int size, int value);

	/// <summary>
	/// Usuwanie macierzy
	/// </summary>
	void deleteMatrix();

	/// <summary>
	/// Metoda ustawiaj�ca parametry pocz�tkowe i tworz�ca macierz dla podanego rozmiaru.
	/// </summary>
	/// <param name="size"></param>
	/// <param name="startNode"></param>
	/// <param name="endNode"></param>
	void setInitials(int size, int startNode, int endNode);

	/// <summary>
	/// �adowanie grafu z pliku
	/// </summary>
	void loadGraphFromFile(std::string fileName);

	/// <summary>
	/// Redukowanie macierzy u�ywane w algorytmie B & B na pocz�tku algorytmu
	/// </summary>
	/// <returns>Suma redukcji</returns>
	int reduceMatrix();

	/// <summary>
	/// Redukowanie wierszy i kolumn macierzy u�ywne w czasie dzia�ania alg. B&B
	/// </summary>
	void reduceColumnsAndRows(int rowNumber, int columnNumber);

	/// <summary>
	/// Klonowanie grafu na podstawie tego podanego jako parametr
	/// </summary>
	/// <param name="graph">Graf do skopiowania</param>
	void clone(Graph* graph);

	int getNumberOfEdges();

	~Graph();

};

