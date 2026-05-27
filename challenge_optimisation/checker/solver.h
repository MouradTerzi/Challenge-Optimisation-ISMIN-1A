#pragma once

#include <iostream>
#include <set>
#include <vector>

using namespace std;
#include "data.h"
#include "solution.h"

class Solver {
public:
	Solver(const MyData& data) { pDat = &data; }
	bool algo(); //true si une solution réalisable est trouvée
	void afficher() const { cout << sol; }
	void save(int numInstance, string fichSol) const { sol.save(numInstance, fichSol); }
private:
	const MyData* pDat;
	Solution sol;
};
