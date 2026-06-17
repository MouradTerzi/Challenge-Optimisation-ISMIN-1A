#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
using namespace std;
#include "data.h"

class Solution
{
public:
    Solution();
    Solution(const MyData& data, const vector<int>& route, const vector<vector<bool>>& tree);
    int getCost() const { return cost; }
    friend ostream& operator<<(ostream& flot, const Solution& sol);
    bool feasible() const;
    void save(int numInstance, string fichSolution) const;
private:
    const MyData* pDat;
    int cost;
    int routeCost;
    int treeCost;
    vector<int> route;
    vector<vector<bool>> tree;
    void dfs(vector<int>& marque, int node) const;
};
