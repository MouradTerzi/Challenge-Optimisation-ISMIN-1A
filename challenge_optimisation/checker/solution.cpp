
using namespace std;
#include "solution.h"

Solution::Solution()
{
}


Solution::Solution(const MyData& data, const vector<int>& route, const vector<vector<bool>>& tree)
{
    this->route = route;
    this->tree = tree;

    pDat = &data;

    // calcul du coût
    routeCost = 0;
    for (int i = 0; i < route.size() - 1; i++)
        routeCost = routeCost + data.getDistance(route[i], route[i + 1]);
    treeCost = 0;
    for (int i = 0; i < data.getNodes(); i++)
        for (int j = 0; j < data.getNodes(); j++)
            if (tree[i][j] == true)
                treeCost = treeCost + data.getDistance(i, j);
    cost = routeCost+treeCost;
}

ostream& operator<<(ostream& flot,const Solution& sol)
{
    flot << "Route (cout "<<sol.routeCost<<") : ";
    for (int i = 0; i < sol.route.size(); i++)
        flot << sol.route[i] << " ";
    flot << endl << endl;

    flot << "Arbre (cout " << sol.treeCost << ") : ";
    for (int i = 0; i < sol.pDat->getNodes(); i++)
        for (int j = 0; j < sol.pDat->getNodes(); j++)
            if (sol.tree[i][j])
                flot << "(" << i << "," << j << ") ";
    flot << endl << endl;
    flot << " Coût total : " << sol.cost << endl<<endl;
    return flot;
}


bool Solution::feasible() const
{
    bool feas = true;

    //COUVERTURE GLOBALE
    vector<bool> present(pDat->getNodes(), false);

    for (int i = 0; i < route.size(); i++)
        present[route[i]] = true;
    for (int i = 0; i < pDat->getNodes(); i++)
        for (int j = 0; j < pDat->getNodes(); j++)
            if (tree[i][j] == true)
            {
                present[i] = true;
                present[j] = true;
            }
    for (int i = 1; i <= pDat->getClients(); i++)
        if (present[i] == false)
        {
            cout << "Erreur solution incomplete (client " << i << ") " << endl;
            feas = false;
        }

    // ROUTE

    //vérification structure route
    if (route.front() != pDat->getVehicleDepot() || route.back() != pDat->getVehicleDepot())
    {
        cout << "Erruer route ne demarre pas ou ne finit pas au depot" << endl;
        feas = false;
    }

    //vérification autonomie route
    if (routeCost> pDat->getVehicleAutonomy())
    {
        cout << "Erreur route trop longue ("<<routeCost<<")" << endl;
        feas = false;
    }

    // ARBRE
    vector<bool> inTree(pDat->getNodes(), false);
    int nbNoeudTree = 0;
    int nbArcTree = 0;

    // initialisation vecteur inTree et comptage du nombre de noeuds
    for (int i = 0; i < pDat->getNodes(); i++)
        for (int j = 0; j < pDat->getNodes(); j++)
            if (tree[i][j] == true)
            {
                if (inTree[i] == false) nbNoeudTree++;
                if (inTree[j] == false) nbNoeudTree++;
                inTree[i] = true;
                inTree[j] = true;
            }

    //vérification doublons dans tree et comptage du nombre d'arcs
    for (int i = 0; i < pDat->getNodes(); i++)
        for (int j = 0; j < pDat->getNodes(); j++)
            if (tree[i][j] == true)
            {
                if (tree[j][i] == true)
                {
                    cout << "Erreur arc en doublon dans l'arbre (" << i << "," << j << ") et (" << j << "," << i << ")" << endl;
                    feas = false;
                }
                else
                    nbArcTree++;
            }

    //vérification nbArc = nbNoeuds-1
    if (nbArcTree != nbNoeudTree - 1)
    {
        cout << "Erreur pipeline n'est pas un arbre (nombre d'arcs != nombre de noeuds - 1)" << endl;
        feas = false;
    }

    // vérification connexite tree et profondeur
    vector<int> marque(pDat->getNodes(), -1);

    marque[pDat->getPipelineSource()] = 0;
    dfs(marque, pDat->getPipelineSource()); //parcours en profondeur depuis la source

    for (int i = 0; i < pDat->getNodes(); i++) //connexité
        if (inTree[i] && marque[i]==-1)
        {
            cout << "Erreur de connexite de l'arbre (sommet " << i << ") " << endl;
            feas = false;
        }

    for (int i = 0; i < pDat->getNodes(); i++) //profondeur
        if (marque[i] > pDat->getPipelineDepth())
        {
            cout << "Erreur profondeur sommet " << i << " dans arbre (" << marque[i] << ")" << endl;
            feas = false;
        }

    // vérification que depot du véhicule n'est pas dans arbre
    if (inTree[pDat->getVehicleDepot()])
    {
        cout << "Erreur depot vehicule dans arbre" << endl;
        feas = false;
    }

    return feas;
}

void Solution::save(int numInstance, string fichSolution) const
{
    ofstream fichier(fichSolution);
    if (!fichier.is_open())
    {
        cerr << "Erreur lors de l'ouverture du fichier solution !" << endl;
        return;
    }

    fichier << "EQUIPE " << MYTEAM << endl;
    fichier << "INSTANCE " << numInstance << endl;
    fichier << "ROUTE ";
    for (int i = 0; i < route.size(); i++)
        fichier << route[i] << " ";
    fichier << endl;
    fichier << "ARBRE ";
    for (int i = 0; i < pDat->getNodes(); i++)
        for (int j = 0; j < pDat->getNodes(); j++)
            if (tree[i][j])
                fichier << "(" << i << "," << j << ") ";
    fichier << endl;
}

void Solution::dfs(vector<int>& marque, int node) const
{
    for (int i = 0; i < pDat->getNodes(); i++)
    {
        if (marque[i] == -1)
            if (tree[node][i] || tree[i][node])
            {
                marque[i] = marque[node]+1;
                dfs(marque, i);
            }
    }
}

