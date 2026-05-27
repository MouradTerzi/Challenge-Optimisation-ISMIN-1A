#include <iostream>
#include "data.h"
#include "solver.h"
#include "checker.h"
#include <filesystem>


using namespace std;

int main(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		Checker CH;

		string filename = argv[i];
		string verification = CH.check(filename);
		cout << verification << endl;
	}
	if (argc == 1)
	{
		Checker CH;
		string fileName;
		cout << "Fichier a verifier : ";
		cin >> fileName;
		cout << CH.check(fileName) << endl;
	}


	return 0;
}
