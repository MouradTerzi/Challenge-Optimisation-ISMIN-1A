#include <sstream>
using namespace std;
#include "team.h"


Team::Team(string d)
{
	dir = d;
	size_t pos = d.find("_");
	eleve = d.substr(0, pos);
	score.resize(NBINSTANCES + 1, INFINI); // les instances commencent � 1
}

// V�rifie et calcule la valeur du r�sultat soumis par l'�quipe dans son r�pertoire pour le fichier name

int Team::compute(string name) // name = nom du fichier (sans le chemin)
{
	Checker CH;
	string resChecker;
	name = getFileName(name); // ajoute le repertoire au nom du fichier
	int val;

	resChecker = CH.check(name);

	if (resChecker!="") // fichier invalide
	{
		if (teamName!="")
			cout << "-> SOURCE = FICHIER " << name << " EQUIPE " << getTeamName() << endl;
		else
			cout << "-> SOURCE = FICHIER " << name << endl;
//		system("pause");

		invalidite.push_back(resChecker + " fichier " + name);
		return 1;
	}
	teamName = CH.getTeamName();
	val = CH.getValue(); // valeur de la solution
	if (DEBUG)
	{
		cout << "Team " << getTeamName() << " : instance " << CH.getNumber() << endl; // CH.getData()->getNumber()
		cout << "Score actuel " << getScore(CH.getNumber()) << endl; // CH.getData()->getNumber()
		cout << "Nouvelle valeur : " << val << endl;
	}
	if (val < getScore(CH.getNumber())) //CH.getData()->getNumber()
	{
		setScore(CH.getNumber(), val); //CH.getData()->getNumber()
		if (DEBUG) cout << "  -> mise � jour" << endl;
	}

	return 0;
}
