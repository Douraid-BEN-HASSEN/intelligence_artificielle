// +------------+
// | EXERCICE_2 |
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
double poids[48];
short retine[48];

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
void calculXj();
void calculPoti();
void calculErreuri();
void correction();

void bruit(int);
void apprentissage(bool);
void generalisation(int, bool);

vector<int> apprentissage_x;
vector<double> apprentissage_y;
vector<int> e0;
vector<int> e1;
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
    //fichier
    ofstream file;
    file.open("apprentissage_x.txt");
    for (int it = 0; it < apprentissage_x.size(); it++) {
        file << apprentissage_x[it] << "\n";
    }
    file.close();

    file.open("apprentissage_y.txt");
    for (int it = 0; it < apprentissage_y.size(); it++) {
        file << apprentissage_y[it] << "\n";
    }
    file.close();

    // gnuplot
    *gp_apprentissage << "set title 'Apprentissage'\n";
    *gp_apprentissage << "set yrange [0:1]\n";
    *gp_apprentissage << "plot '-', '-' with lines title 'courbe'\n ";
    gp_apprentissage->send(apprentissage_y);
    gp_apprentissage->send(apprentissage_x);
    // graph generalisation

    //fichier
    ofstream f_b;
    f_b.open("b.txt");
    for (int it = 0; it < bruit_g.size(); it++) {
        f_b << bruit_g[it] << "\n";
    }
    f_b.close();

    ofstream f_e0;
    f_e0.open("e0.txt");
    for (int it = 0; it < e0.size(); it++) {
        f_e0 << e0[it] << "\n";
    }
    f_e0.close();

    ofstream f_e1;
    f_e1.open("e1.txt");
    for (int it = 0; it < e1.size(); it++) {
        f_e1 << e1[it] << "\n";
    }
    f_e1.close();

    // gnuplot
    *gp_generalisation << "set title 'Generalisation'\n";
    *gp_generalisation << "plot '-', '-' with lines title 'e0', ";
    *gp_generalisation << "'-', '-' with lines title 'e1'\n ";
    gp_generalisation->send(e0);
    gp_generalisation->send(bruit_g);
    gp_generalisation->send(e1);
    gp_generalisation->send(bruit_g);

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
    int hasard = std::rand() % 100;
    
    string line;
    ifstream myfile;
    int index = 0;

    if (hasard > 50) {
        myfile.open("./data/1/un.txt");
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
    else {
        myfile.open("./data/0/zero.txt");
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
        entree_x[sizeof(entree_x)-1] = 0;
    }
}

void setClass(char c) {
    string line;
    ifstream myfile;
    int index = 0;

    if (c == 1) myfile.open("./data/1/un.txt");
    else myfile.open("./data/0/zero.txt");
    
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
    entree_x[sizeof(entree_x) - 1] = c;
}

void initPoids() {
    int length = sizeof(poids)/ sizeof(*poids);
    double random;

    for (int it = 0; it < length; it++) {
        random = ((double) rand()) / (double) RAND_MAX;
        poids[it] =  random/48.0;
    }
}

void calculXj() {
    int length = sizeof(entree_x) - 1;
    for (int it = 0; it < length; it++) {
        if (entree_x[it] == '.') retine[it] = 0;
        else retine[it] = 1;
    }
}

void calculPoti() {
    int length = sizeof(retine) / sizeof(*retine);
    poti = 0;
    for (int it=0; it<length; it++) {
        poti += poids[it] * (double)retine[it];
    }
}

void calculErreuri() {
    erreuri = ((double)entree_x[sizeof(entree_x) - 1] - poti);
}

void correction() {
    int length = sizeof(poids) / sizeof(*poids);

    for (int it = 0; it < length; it++) {
        poids[it] = poids[it] + (eps * erreuri *  retine[it]);
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
    setRandomClass();
    initPoids();

    bool stop = false;

    double err0 = 0;
    double err1 = 0;
    double erreurTotale = 99;
    
    // seuil 0.0001
    while (!(erreurTotale <= 0.0001))
    {
        //check 1
        setClass(1);
        calculXj(); // 1
        calculPoti(); // 2
        calculErreuri(); // 3
        correction();
        err1 = erreuri;

        // check 0
        setClass(0);
        calculXj(); // 1
        calculPoti(); // 2
        calculErreuri(); // 3
        correction();
        err0 = erreuri;

        erreurTotale = err0 + err1;

        if (displayResult) {
            cout << "poti = " << poti << endl;
            cout << "erreuri = " << erreuri << endl;
            cout << "iteration = " << iteration << endl;
            cout << "erreur totale = " << erreurTotale << endl << endl;
        }

        iteration++;
        apprentissage_x.push_back(iteration);
        apprentissage_y.push_back(erreurTotale);
    }
}

void generalisation(int pVal, bool displayResult = true) {
    bruit_g.push_back(pVal);

    poti = 0.0;
    bool stop = false;
    int Err0 = 0;
    int Err1 = 0;

    // check 1
    for (int it = 0; it < 50; it++) {
        setClass(1);
        bruit(pVal); // application du bruit
        calculXj(); // 1
        calculPoti(); // 2
        calculErreuri(); // 3
        if (!(erreuri >= -0.1 && erreuri <= 0.1)) Err1++;
    }

    // check 0
    for (int it = 0; it < 50; it++) {
        setClass(0);
        bruit(pVal); // application du bruit
        calculXj(); // 1
        calculPoti(); // 2
        calculErreuri(); // 3
        if (!(erreuri >= -0.2 && erreuri <= 0.2)) Err0++;
    }

    e0.push_back(Err0);
    e1.push_back(Err1);

    stop = true;
    if (displayResult) {
        cout << "Bruit = " << pVal << " | Err0 = " << Err0 << " | Err1 = " << Err1 << endl << endl;
        cout << endl;
    }
}
