// +------------+
// | EXERCICE_3 |
// +------------+

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "gnuplot-iostream.h"

using namespace std;

unsigned char entree_x[49];
double poids[10][48];
short retine[10][48];

const double teta = 0.5;
const double eps = 0.01;
double poti = 0;
double erreuri = 0;
int iteration = 0;
int nbErreur = 0;
void init();
void setRandomClass();
void setClass(char);
void initPoids();
void calculXj(int);
void calculPoti(int);
void calculErreuri();
void correction(int);

void bruit(int);
void apprentissage(bool);
void generalisation(int, bool);

vector<int> apprentissage_x[10];
vector<double> apprentissage_y[10];
vector<int> erreur_g[10];
vector<int> bruit_g;

Gnuplot* gp_apprentissage;
Gnuplot* gp_generalisation;

int main()
{
    init();

    // +---------------+
    // | APPRENTISSAGE |
    // +---------------+
    cout << "+---------------+" << endl;
    cout << "| APPRENTISSAGE |" << endl;
    cout << "+---------------+" << endl;
    apprentissage(true);
    // =================

    // +----------------+
    // | GENERALISATION |
    // +----------------+
    cout << "=============================================" << endl;
    cout << "+----------------+" << endl;
    cout << "| GENERALISATION |" << endl;
    cout << "+----------------+" << endl;

    for (int it = 2; it < 51; it++) {
        generalisation(it, true);
    }
    // ==================

    // graph apprentissage
    // fichier
    ofstream file;
    for (int iTest = 0; iTest < 10; iTest++) {
        file.open("apprentissage_x_" + std::to_string(iTest) + ".txt");
        for (int it = 0; it < apprentissage_x[iTest].size(); it++) {
            file << apprentissage_x[iTest][it] << "\n";
        }
        file.close();
    }

    for (int iTest = 0; iTest < 10; iTest++) {
        file.open("apprentissage_y_" + std::to_string(iTest) + ".txt");
        for (int it = 0; it < apprentissage_y[iTest].size(); it++) {
            file << apprentissage_y[iTest][it] << "\n";
        }
        file.close();
    }

    // gnuplot
    *gp_apprentissage << "set title 'Apprentissage'\n";
    *gp_apprentissage << "set yrange [0:1]\n";
    *gp_apprentissage << "plot ";
    for (int iTest = 0; iTest < 10; iTest++) {
        if (iTest != 9) *gp_apprentissage << "'-', '-' with lines title 'chiffre_" << iTest << "', ";
        else *gp_apprentissage << "'-', '-' with lines title 'chiffre_" << iTest << "'\n ";
    }
    for (int iTest = 0; iTest < 10; iTest++) {
        gp_apprentissage->send(apprentissage_y[iTest]);
        gp_apprentissage->send(apprentissage_x[iTest]);
    }

    *gp_generalisation << "set title 'Generalisation'\n";
    *gp_generalisation << "plot ";
    for (int iTest = 0; iTest < 10; iTest++) {
        if (iTest != 9) *gp_generalisation << "'-', '-' with lines title 'chiffre_" << iTest << "', ";
        else *gp_generalisation << "'-', '-' with lines title 'chiffre_" << iTest << "'\n ";
    }
    for (int iTest = 0; iTest < 10; iTest++) {
        gp_generalisation->send(erreur_g[iTest]);
        gp_generalisation->send(bruit_g);
    }

    ofstream f_e;
    for (int i = 0; i < 10; i++) {
        f_e.open("e_" + std::to_string(i) + ".txt");
        for (int it = 0; it < erreur_g[i].size(); it++) {
            f_e << erreur_g[i][it] << "\n";
        }
        f_e.close();
    }

    std::cin.get();
}

void init() {
    std::srand(std::time(NULL));
#ifdef _WIN32     
    gp_apprentissage = new Gnuplot("\"D:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");
    gp_generalisation = new Gnuplot("\"D:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");
#else
    gp_apprentissage = new Gnuplot();
    gp_generalisation = new Gnuplot();
#endif
}

void setRandomClass() {
    int hasard = std::rand() % 10;

    string line;
    ifstream myfile;
    int index = 0;

    if (hasard == 0) myfile.open("./data/zero.txt");
    else if (hasard == 1) myfile.open("./data/un.txt");
    else if (hasard == 2) myfile.open("./data/deux.txt");
    else if (hasard == 3) myfile.open("./data/trois.txt");
    else if (hasard == 4) myfile.open("./data/quatre.txt");
    else if (hasard == 5) myfile.open("./data/cinq.txt");
    else if (hasard == 6) myfile.open("./data/six.txt");
    else if (hasard == 7) myfile.open("./data/sept.txt");
    else if (hasard == 8) myfile.open("./data/huit.txt");
    else if (hasard == 9) myfile.open("./data/neuf.txt");

    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            for (int it = 0; it < line.length(); it++) {
                entree_x[index + it] = line[it];
            }
            index += line.length();
        }
        myfile.close();
    }
    else cout << "Unable to open file";
    entree_x[sizeof(entree_x) - 1] = 1;
}

void setClass(char c) {
    string line;
    ifstream myfile;
    int index = 0;

    if (c == 0) myfile.open("./data/zero.txt");
    else if (c == 1) myfile.open("./data/un.txt");
    else if (c == 2) myfile.open("./data/deux.txt");
    else if (c == 3) myfile.open("./data/trois.txt");
    else if (c == 4) myfile.open("./data/quatre.txt");
    else if (c == 5) myfile.open("./data/cinq.txt");
    else if (c == 6) myfile.open("./data/six.txt");
    else if (c == 7) myfile.open("./data/sept.txt");
    else if (c == 8) myfile.open("./data/huit.txt");
    else if (c == 9) myfile.open("./data/neuf.txt");

    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            for (int it = 0; it < line.length(); it++) {
                entree_x[index + it] = line[it];
            }
            index += line.length();
        }
        myfile.close();
    }
    else cout << "Unable to open file";
    entree_x[sizeof(entree_x) - 1] = 1;
}

void initPoids() {
    double random;

    for (int iNeurone = 0; iNeurone < 10; iNeurone++) {
        for (int it = 0; it < 48; it++) {
            random = ((double)rand()) / (double)RAND_MAX;
            poids[iNeurone][it] = random / 48.0;
        }
    }
}

void calculXj(int p_iNeurone) {
    int length = sizeof(entree_x) - 1;
    for (int it = 0; it < length; it++) {
        if (entree_x[it] == '.') retine[p_iNeurone][it] = 0;
        else retine[p_iNeurone][it] = 1;
    }
}

void calculPoti(int p_iNeurone) {
    //int length = sizeof(retine) / sizeof(*retine);
    poti = 0;
    for (int it = 0; it < 48; it++) {
        poti += poids[p_iNeurone][it] * (double)retine[p_iNeurone][it];
    }
}

void calculErreuri() {
    erreuri = ((double)entree_x[sizeof(entree_x) - 1] - poti);
}

void correction(int p_iNeurone) {
    for (int it = 0; it < 48; it++) {
        poids[p_iNeurone][it] = poids[p_iNeurone][it] + (eps * erreuri * retine[p_iNeurone][it]);
    }
}

void bruit(int pVal) {
    pVal /= 2;

    int length = sizeof(entree_x) - 1;
    int indexRand;
    bool indexAllowed;
    vector<int> already;

    while (pVal > 0)
    {
        indexAllowed = false;
        while (!indexAllowed)
        {
            indexRand = rand() % length;
            indexAllowed = !std::count(already.begin(), already.end(), indexRand);
        }

        if (entree_x[indexRand] == '.') entree_x[indexRand] = '*';
        else entree_x[indexRand] = '.';

        already.push_back(indexRand);

        pVal--;
    }
}

void apprentissage(bool displayResult = true) {
    initPoids();

    double erreurTotale[10] = { 99,99,99,99,99,99,99,99,99,99 };

    int hasard = 0;

    // seuil 0.0001
    while (!(erreurTotale[0] <= 0.0001) ||
        !(erreurTotale[1] <= 0.0001) ||
        !(erreurTotale[2] <= 0.0001) ||
        !(erreurTotale[3] <= 0.0001) ||
        !(erreurTotale[4] <= 0.0001) ||
        !(erreurTotale[5] <= 0.0001) ||
        !(erreurTotale[6] <= 0.0001) ||
        !(erreurTotale[7] <= 0.0001) ||
        !(erreurTotale[8] <= 0.0001) ||
        !(erreurTotale[9] <= 0.0001))
    {
        // set random class
        hasard = std::rand() % 10;
        setClass(hasard);
        calculXj(hasard); // 1
        calculPoti(hasard); // 2
        calculErreuri(); // 3
        correction(hasard);

        erreurTotale[hasard] = erreuri;

        if (displayResult) {
            cout << "poti = " << poti << endl;
            cout << "erreuri = " << erreuri << endl;
            cout << "iteration = " << iteration << endl;
            cout << "erreur totale = " << erreurTotale[hasard] << endl << endl;
        }

        iteration++;
        apprentissage_x[hasard].push_back(iteration);
        apprentissage_y[hasard].push_back(erreurTotale[hasard]);
    }
}

void generalisation(int pVal, bool displayResult = true) {
    bruit_g.push_back(pVal);

    poti = 0.0;
    bool stop = false;
    int Err = 0;

    // check all
    for (int iTest = 0; iTest < 10; iTest++) {
        for (int it = 0; it < 50; it++) {
            setClass(iTest);
            bruit(pVal); // application du bruit
            calculXj(iTest); // 1
            calculPoti(iTest); // 2
            calculErreuri(); // 3
            if (!(erreuri >= -0.1 && erreuri <= 0.1)) Err++;
        }
        erreur_g[iTest].push_back(Err);
    }
}