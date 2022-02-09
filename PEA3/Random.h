#pragma once
class Random
{
public:
	/// <summary>
	/// Losowanie liczby ca�kowitej z podanego przedzia�u
	/// </summary>
	/// <param name="lowerBound">Dolna granica</param>
	/// <param name="upperBound">G�rna granica</param>
	/// <returns>Wylosowana liczba</returns>
	static int  generateRandomNumber(int lowerBound, int upperBound);

};

