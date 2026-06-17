#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include<list>
#include<vector>

using namespace std;

#include "data.h"
#include "solution.h"

class Checker
{
public:
    int getValue() { return value; }
    string getTeamName() { return team; }
    int getNumber() { return number; }
    Solution getSol() { return solution; }
    string check(string& fileName);     //Vérification de la faisabilité de la solution
private:
    Solution solution;
    int value;
    string team;
    int number;
    bool entete(ifstream& file); //Vérification de l'en-tête
};

