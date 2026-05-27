
#include <fstream>
#include <sstream>
using namespace std;
#include "data.h"


// construction from filename f, restricted to nbClients
// 0 is the vehicle depot
// customers are numbered from 1 to nbclients
// nbClients + 1 is the pipeline source (101 in the instance file)

MyData::MyData(string f)
{
		ifstream dataFile;
		string temp;

		dataFile.open(f);
		if (!dataFile) cout << "FILE NOT FOUND" << endl;

		dataFile >> temp >> nbClients;
		nbNodes = nbClients + 2;
		distance.resize(nbNodes);
		for (int i = 0; i < nbNodes; i++)
			distance[i].resize(nbNodes);
		X.resize(nbNodes);
		Y.resize(nbNodes);
		
		dataFile >> temp>>temp>>vehicleAutonomy ;
		dataFile >> temp >> temp >> pipelineDepth;

		dataFile >> temp >> temp >> temp;
		dataFile >> temp >> X[0] >> Y[0];

		dataFile >> temp >> temp;
		for (int i = 1; i <= nbClients; i++)
			dataFile >> temp >> X[i] >> Y[i];

		dataFile >> temp >> temp>>temp;
		dataFile >> temp >> X[nbClients+1] >> Y[nbClients+1];

		dataFile.close();

		for (int i = 0; i < nbNodes; i++)
			for (int j = 0; j < nbNodes; j++)
				if (i == j)
					distance[i][j] = INFINI;
				else
					distance[i][j] = abs(X[i] - X[j]) + abs(Y[i] - Y[j]); // distance de Manhattan
}

	void MyData::generate(int num) 
	{
		string name;
		ostringstream flux;

		flux << "instance";
		flux << num;
		flux << ".txt";
		name = flux.str();
		ofstream newFile(name); // ouvre le fichier de sortie, avec le nom name 

		newFile << "CUSTOMERS " << 100 << endl;
		newFile << "VEHICLE AUTONOMY " << 500 << endl;
		newFile << "PIPELINE DEPTH " << 3 << endl << endl;

		newFile << "VEHICLE DEPOT : " << endl << "0 " << 25 << " " << 25 << endl << endl;

		newFile << "CUSTOMERS : " << endl;

		vector<int> x(101), y(101);
		for (int i = 1; i <= 100; i++)
		{
			x[i] = (int)rand() % 100 + 1;
			y[i] = (int)rand() % 100 + 1;
			// si un noeud est déja à cette position, on tire de nouvelles valeurs (en décrémentant i)
			for (int j = 1; j < i; j++)
				if (x[j] == x[i] && y[j] == y[i])
					i--;
			if (x[i] == 25 && y[i] == 25)
				i--;
			if (x[i] == 75 && y[i] == 75)
				i--;
		}

		for (int i = 1; i <= 100; i++)
			newFile << i << " " << x[i] << " " << y[i] << endl;
		newFile << endl;

		newFile << "PIPELINE SOURCE : " << endl << 101 << " " << 75 << " " << 75 << endl;
	}



