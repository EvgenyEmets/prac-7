#ifndef TASK_2_CSV_MOD_H
#define TASK_2_CSV_MOD_H

#include "solution.h"
#include <ctime>
#include <random>
#include <fstream>

void generate (int NWork, int NProc, int TimeLim1, int TimeLim2) {
    std::srand(std::time(nullptr));
    std::ofstream data;
    data.open("data.csv");
    data << NWork << "," << NProc << std::endl;
    for (int i = 0; i < NWork; i++) {
        data << i << "," << std::rand() % (TimeLim2 - TimeLim1 + 1) + TimeLim1 << std::endl;
    }
    data.close();
}

void encode (int &NWork, int &NProc, std::vector<Work> &Works) {
    std::ifstream data;
    data.open("data.csv");
    NWork = 0;
    NProc = 0;
    Works.clear();
    std::vector<int> tmpV;
    int VP = 0;
    char tmp;
    data.read((&tmp), 1);
    while (!data.eof()) {
        tmpV.push_back(0);
        while (tmp != ',' && tmp != 10) {
            tmpV[VP] = tmpV[VP] * 10 + tmp - '0';
            data.read((&tmp), 1);
        }
        VP++;
        data.read((&tmp), 1);
    }
    NWork = tmpV[0];
    NProc = tmpV[1];
    VP = 2;
    for (int i = 0; i < NWork; i++) {
        Works.emplace_back(Work(tmpV[VP], tmpV[VP + 1]));
        VP += 2;
    }
    tmpV.clear();
    data.close();
}

#endif //TASK_2_CSV_MOD_H
