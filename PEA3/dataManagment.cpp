#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <Windows.h>
#include "dataManagment.h"
using namespace std;


    


    int** getData(string name)
    {
        int** matrix = NULL;

        int size;
        fstream file;
        string line;
        file.open(name, ios::in);
        if (!file.good()) {
            cout << "\nBlad wczytywania pliku.\n";
            file.close();
            file.clear();
        }
        else {

            getline(file, line);
            getline(file, line);
            getline(file, line);
            getline(file, line);
            line.erase(0, 11);
            cout << line << endl;
            size = stoi(line);
            cout << "\nIlosc wierzcholkow: " << size << "\n";
            getline(file, line);
            getline(file, line);
            getline(file, line);

            matrix = new int* [size];
            for (int i = 0; i < size; i++) {
                matrix[i] = new int[size];
            }
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    file >> matrix[i][j];
                }
            }
            matrix[0][0] = size;
        }
        file.close();
        file.clear();
        return matrix;
    }

    void showData(int** matrix, int size)
    {
        cout << "IMMA SHOW " << endl;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cout << matrix[i][j]<<" ";
            }
            cout << endl;
        }

    }



