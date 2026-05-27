#pragma once
#include <vector>
#include<string>
#include<fstream>
#include<iostream>
using namespace std;
#include "data.h"
#include "checker.h"*/

class Team
{
public:
	Team(string d);
	string getDir() const { return dir; }
	int getScore(int i) const { return score[i]; }
	void setScore(int i, int j) { score[i] = j; }
	string getFileName(string name) const { return getDir() + string("/") + name ; }
	string getTeamName() const { return teamName; }
	void setTeamName(string s) { teamName = s; } //utile pour diriger vers un repertoire special quand nom equipe pas connu
	string getEleve() const { return eleve; }
	int compute(string name);
	int numberInvalidity() const { return (int) invalidite.size(); }
	string getInvalidity(int i) const { return invalidite[i]; }
private:
	string dir;	// r�pertoire o� trouver les fichiers r�sultats de l'�quipe
	string eleve; // obtenu dans le debut du nom du repertoire suite � extraction campus
	string teamName; // obtenu dans les fichiers r�sultats
	vector<int> score; // scores obtenus sur les instances (INFINI si pas de score)
	vector<string> invalidite;
};
