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
    int matrixSize = 0, menu, popMenu = 0, genMenu;
    int** matrix = NULL;
    int stopTime = 0;
    double mutPercMenu = 0.01;
    double xPercenu = 0.8;
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
            genMenu = showGenMenu();

            if (genMenu == 0)
            {
                Genetic gen;
                gen.setMatrix(matrix, matrixSize);

                gen.setGenetic(stopTime, popMenu, xPercenu, mutPercMenu,xMenu);
                gen.startGenetic();
                gen.saveToFileGenetic();
                break;
            }
            else if (genMenu == 1)
            {
                for (int i = 0;i < 10;i++)
                {
                    Genetic gen;
                    gen.setMatrix(matrix, matrixSize);

                    gen.setGenetic(stopTime, popMenu, xPercenu, mutPercMenu,xMenu);
                    gen.startGenetic();
                    gen.saveToFileGenetic();
                }
                break;

            }
            else
                break;
        }
        case 9: {
            //TESTORY
            string files[] = { "rbg403.atsp", "ftv170.atsp", "ftv47.atsp" };
            int times[] = { 180, 120, 60 };

            for (int rodzajCrossa = 0;rodzajCrossa < 2;rodzajCrossa++)
            {
                for (int wielkoscEpoki; wielkoscEpoki < 3; wielkoscEpoki++)
                {
                    for (int i = 0;i < 3;i++)
                    {
                        for (int j = 0;j < 10;j++)
                        {
                            Genetic gen;

                            matrix = getData(files[i]);
                            matrixSize = matrix[0][0];
                            matrix[0][0] = matrix[1][1];

                            gen.setMatrix(matrix, matrixSize);

                            gen.setGenetic(times[i], matrixSize * wielkoscEpoki, 0.8, 0.01, rodzajCrossa);
                            gen.startGenetic();
                            gen.saveToFileGenetic();
                        }
                    }
                }
            }

            break;
        }

        case 0: {
            system("pause");
            break;
        }
        }
    } while (menu != 0);


}

