#include "checker.h"

using namespace std;


//Lecture de l'entete du fichier solution (déjà ouvert)
// identifie team et number
bool Checker::entete(ifstream& file)
{
    string ligne, temp;

    // Identification équipe
    getline(file, ligne);
    istringstream issEquipe(ligne);
    issEquipe >> temp;
    getline(issEquipe, team);
    if (temp != "EQUIPE")
    {
        cout << "Erreur absence mot cle EQUIPE" << endl;
        return false;
    }

    // Identification instance
    getline(file, ligne);
    istringstream issInstance(ligne);
    issInstance >> temp >> number;
    if (temp != "INSTANCE")
    {
        cout << "Erreur absence mot cle INSTANCE" << endl;
        return false;
    }

    // vérification que l'instance existe
    if (number < 0 || number > NBINSTANCES)
    {
        cout << "Erreur lecture numero instance (" << number <<")"<< endl;
        return false;
    }

    return true;
}

//Vérifier la faisabilité de la solution
string Checker::check(string& fileName)
{
    ifstream file(fileName);
    string ligne,temp,node1,node2;
    char parenthese;
    bool ok;
    int i,j;

    //cout << "VERIFICATION SOLUTION " << fileName << endl;

    if (!file)
    {
        cout << "Erreur ouverture du fichier solution" << endl;
        return "Erreur : Solution irréalisable";
    }

    ok = entete(file);
    if (!ok) return "Erreur : Solution irréalisable";

    //cout << "INSTANCE " << number << " TEAM " << team << endl << endl;

    // Ouvre l'instance et crée l'objet Data
    ostringstream flux;
    flux << "../media/instances/instance" << number << ".txt";
    temp = flux.str();
    MyData data(temp);

    //read et check structure route (autonomie vérifiée dans sol.feasible())
    vector<int> route;
    getline(file, ligne);
    istringstream issRoute(ligne);
    issRoute >> temp;
    if (temp != "ROUTE")
    {
        cout << "Erreur absence mot cle ROUTE" << endl;
        return "Erreur : Solution irréalisable";
    }
    issRoute >> i;
    route.push_back(i);
    if (i != data.getVehicleDepot())
    {
        cout << "Erreur la route ne commence pas par le depot"<<endl;
        return "Erreur : Solution irréalisable";
    }
    while (issRoute >> i)
    {
        if (i<0 || i> data.getClients() + 1)
        {
            cout << "Erreur la route contient un client invalide (" << i << ")" << endl;
            return "Erreur : Solution irréalisable";
        }
        else
        {
            route.push_back(i);
        }
    }
    if (route.back() != data.getVehicleDepot())
    {
        cout << "Erreur la route ne finit pas par le depot" << endl;
        return "Erreur : Solution irréalisable";
    }

    //read arbre (structure de l'arbre et profondeur vérifiées dans sol.feasible())
    vector<vector<bool>> tree;
    tree.resize(data.getNodes());
    for (int i = 0; i < data.getNodes(); i++)
        tree[i].resize(data.getNodes());
    for (int i = 0; i < data.getNodes(); i++)
        for (int j = 0; j < data.getNodes(); j++)
            tree[i][j] = false;

    getline(file, ligne);
    istringstream issTree(ligne);
    issTree >> temp;
    if (temp != "ARBRE")
    {
        cout << "Erreur absence mot cle ARBRE" << endl;
        return "Erreur : Solution irréalisable";
    }
    while (issTree >> parenthese)
    {
        if (parenthese != '(') //lit parenthèse
        {
            cout << "Erreur au moins un arc de l'arbre mal formatte (mauvais delimiteur)" <<endl;
            return "Erreur : Solution irréalisable";
        }
        getline(issTree, node1, ','); // lit premier sommet
        getline(issTree, node2, ')'); // lit second sommet

        try
        {
            i = stoi(node1);
            j = stoi(node2);
            if (i<0 || i> data.getClients() + 1 || j<0 || j> data.getClients() + 1)
            {
                cout << "Erreur l'arbre contient un arc invalide (" << i << "," << j << ")" << endl;
                return "Erreur : Solution irréalisable";
            }
            else
                tree[i][j] = true;
        }
        catch (const invalid_argument& e)
        {
            cout << "Erreur dans la lecture d'un arc de l'arbre : mauvais délimiteur ou valeur incorrecte" << endl;
            return "Erreur : Solution irréalisable";
        }
    }

    solution = Solution(data,route, tree);
    ok = solution.feasible();

    value = INFINI;
    if (ok)
        value = solution.getCost();

    if (ok)
    {
        //cout << "----> Solution realisable de valeur " << value << endl << endl;
        return to_string(value);
    }
    else
    {
        //cout << "----> Solution irrealisable" << endl << endl;
        return "Erreur : Solution irréalisable";
    }
}

