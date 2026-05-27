using namespace std;
#include "solver.h"

bool Solver::algo()
{
	int bestClient;
	int bestDistance;
	vector<int> route;
	int last;
	int residualAutonomy=pDat->getVehicleAutonomy();
	int out = pDat->getClients();
	vector<bool> inRoute(pDat->getNodes(), false);

	route.push_back(pDat->getVehicleDepot());
	last = pDat->getVehicleDepot();
	inRoute[pDat->getVehicleDepot()]=true;

	do
	{
		bestClient = -1;
		bestDistance = INFINI;
		for (int i=1;i<= pDat->getClients();i++)
			if (!inRoute[i] && pDat->getDistance(last, i) < bestDistance && pDat->getDistance(last, i) + pDat->getDistance(i, pDat->getVehicleDepot()) <= residualAutonomy)
			{
				bestClient = i;
				bestDistance = pDat->getDistance(last, i);
			}
		if (bestClient != -1)
		{
			route.push_back(bestClient);
			inRoute[bestClient] = true;
			out--;
			residualAutonomy = residualAutonomy - pDat->getDistance(last, bestClient);
			last = bestClient;
		}
	} while (bestClient != -1);

	route.push_back(pDat->getVehicleDepot());

	vector<vector<bool>> tree;
	vector<pair<bool,int>> inTree(pDat->getNodes(), make_pair(false,0));
	int bestAttach;

	tree.resize(pDat->getNodes());
	for (int i = 0; i < pDat->getNodes(); i++)
		tree[i].resize(pDat->getNodes());

	inTree[pDat->getPipelineSource()] = make_pair(true, 0);

	for (int i = 0; i < pDat->getNodes(); i++)
		for (int j = 0; j < pDat->getNodes(); j++)
			tree[i][j] = false;

	while (out > 0)
	{
		bestClient = -1;
		bestDistance = INFINI;
		bestAttach = 0;
		for (int i = 0; i < pDat->getNodes(); i++)
			for (int j = 0; j < pDat->getNodes(); j++)
				if (inTree[i].first && inTree[i].second < pDat->getPipelineDepth() && !inTree[j].first && !inRoute[j] && pDat->getDistance(i, j) < bestDistance)
				{
					bestAttach = i;
					bestClient = j;
					bestDistance = pDat->getDistance(i, j);
				}
		tree[bestAttach][bestClient] = true;
		inTree[bestClient] = make_pair(true,inTree[bestAttach].second+1);
		out--;
	}
	sol = Solution(*pDat, route, tree);
	return sol.feasible();
}