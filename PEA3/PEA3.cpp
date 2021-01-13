
#include <iostream>
#include <string>
#include "menu.h"
#include "dataManagment.h"
#include "Czas.h"
#include "Genetic.h"


int main()
{
    srand(time(NULL));
    std::cout << "Hello World!\n";
    int matrixSize = 0, menu, popMenu = 0;
    int** matrix = NULL;
    int stopTime = 0;
    double mutPercMenu = 0.8;
    double xPercenu = 0.01;
    int xMenu = 0;
    int mMenu = 0;
    string file;

    do
    {
        menu = showMenu();
        system("cls");
        switch (menu) {
        case 1: {
            string name;
            cout << "==============================================================" << endl;
            cout << "Wprowadz nazwe pliku: " << endl;
            cout << "==============================================================" << endl;
            //name = "ftv47.atsp";
            cin >> name;
            matrix = getData(name);
            matrixSize = matrix[0][0];
            matrix[0][0] = matrix[1][1];
            //showData(matrix,matrixSize);
            break;
        }
        case 2: {
            cout << "Aktualne kryterium stopu: " << stopTime << endl;
            stopTime = showStopTimeMenu();
            break;
        }
        case 3: {
            cout << "Aktualna wielkosc populacji poczatkowej: " << popMenu << endl;
            popMenu = showPopMenu();
            break;
        }
        case 4: {
            cout << "Aktualny wspolczynnik mutacji: " << mutPercMenu << endl;
            mutPercMenu = showMutPercMenu();
            break;
        }
        case 5: {
            cout << "Aktualny wspolczynnik krzyzowania: " << xPercenu << endl;
            xPercenu = showXPercMenu();
            break;
        }
        case 6: {
            cout << "Aktualny wybor : " << xMenu << endl;
            xMenu = showXMenu();
            break;
        }
        case 7: {
            cout << "Aktualny wybor: " << mMenu << endl;
            mMenu = showMMenu();
            break;
        }
        case 8: {
            Genetic gen;
            gen.setMatrix(matrix, matrixSize);
            cout << "START" << endl;
            

            break;
        }

        case 0: {
            system("pause");
            break;
        }
        }
    } while (menu != 0);


}

