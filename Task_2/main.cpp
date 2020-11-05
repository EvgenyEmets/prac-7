#include <cmath>
#include <vector>
#include <list>
#include <ctime>
#include <random>
#include <iostream>
#include <thread>
#include <fstream>
#include "temperature.h"
#include "solution.h"
#include "mutation.h"
#include "simulation.h"
#include "csv_mod.h"

void func (Shed_Len *SInit, Mutation_R *MInit, Boltzmann *TInit, Shed_Len **Res, int thr) {
    Shed_Len *S = dynamic_cast<Shed_Len*>(SInit->cpObj());
    Boltzmann *T = dynamic_cast<Boltzmann *>(TInit->cpObj());
    Mutation_R *M = dynamic_cast<Mutation_R *>(MInit->cpObj());
    SimAnn A(S, T, M);
    *Res = dynamic_cast<Shed_Len*>(A.Simulate());
    delete T;
    delete M;
}



int main() {
    std::vector<Work> W;
    int NWork, NProc;
    encode(NWork, NProc, W);
    Shed_Len* S;
    S = new Shed_Len(NWork, NProc, W);
    Mutation_R M;
    Boltzmann T(100);
    int Stop = 0;
    //SimAnn A(S, &T, &M);
    //Shed_Len *Ans = dynamic_cast<Shed_Len*>(A.Simulate());
    //Ans->printSol();
    //delete Ans;
    int NTread = 4;
    std::vector<std::thread> ThreadVector;
    int BestMetric = S->getCritVal();
    while (Stop < 10) {
        std::vector<Shed_Len*> Buffer(NTread);
        for (int i = 0; i < NTread; i++) {
            ThreadVector.emplace_back(func, S, &M, &T, &(Buffer[i]), i);
        }
        for (int i = 0; i < NTread; i++) {
            ThreadVector[i].join();
        }
        delete S;
        //Buffer[0]->printSol();
        Shed_Len *BestSolution = dynamic_cast<Shed_Len*>(Buffer[0]->cpObj());
        for (int i = 0; i < NTread; i++) {
            if (BestMetric > Buffer[i]->getCritVal()) {
                Stop = 0;
                delete BestSolution;
                BestSolution = dynamic_cast<Shed_Len*>(Buffer[i]->cpObj());
                BestMetric = BestSolution->getCritVal();
            }
            //Buffer[i]->printSol();
            delete Buffer[i];
        }
        S = BestSolution;
        Buffer.clear();
        ThreadVector.clear();
        Stop++;
    }
    S->printSol();
    std::cout << S->getCritVal() << std::endl;
    std::cout << "PROGRAM END" << std::endl;
    delete S;

    return 0;
}