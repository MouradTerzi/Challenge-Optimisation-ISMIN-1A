#pragma once
#include <vector>
#include <string>
#include <iostream>
#include<time.h>

using namespace std;

const int NBINSTANCES = 15;
const string MYTEAM = "Ze p_RO_f"; // pour cr�er des fichiers r�sultats et d�bugguer
const bool DEBUG = true; // pour le log (attention mode debug ne fonctionne pas pour un usage normal)
const int INFINI = 1000000000; // pour le log (valeur par d�faut des scores pour les instances : �tre plus grand que les valeurs de l'heuristique suffit)


class MyData
{
public:
	MyData(string f); //constructor (the data is limited to the first nbClients)
	int getNodes() const { return nbNodes; } //number of nodes: depot for the vehicle + source for the pipeline + customers
	int getClients() const { return nbClients; } // number of customers
	int getVehicleAutonomy() const { return vehicleAutonomy; } //vehicle autonomy
	int getPipelineDepth() const { return pipelineDepth; } // pipeline depth
	int getDistance(int i, int j) const { return distance[i][j]; }
	int getVehicleDepot() const { return 0; }
	int getPipelineSource() const { return nbClients + 1; }
	static void generate(int num); //generate the benchmark instances
private:
	int nbClients;
	int nbNodes;  // nbCustomers + 2 (depot for the vehicle, source for the pipeline pipeline)
	int vehicleAutonomy; // vehicle autonomy
	int pipelineDepth; // maximal depth of the pipeline tree
	vector<vector<int>> distance; // distance matrix
	vector<int> X, Y; //coordonnés
};
