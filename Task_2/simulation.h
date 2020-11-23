#ifndef TASK_2_SIMULATION_H
#define TASK_2_SIMULATION_H

#include "mutation.h"
#include "solution.h"
#include "temperature.h"

class SimAnn {
    Solution *S;
    Temperature *T;
    Mutation *M;
    Solution *Best;
    int Crit;
    int Stop;

public:
    SimAnn (Solution *S0, Temperature *T0, Mutation *M0) : S(S0), T(T0), M(M0) {
        Best = S->cpObj();
        Crit = Best->getCritVal();
        Stop = 0;
    }
    Solution *Simulate() {
        std::srand(std::time(nullptr));
        while (Stop < 100) {
            Stop++;
            for (int i = 0; i < 20; i++) {
                Solution *MutateSol = M->Mutate(S->cpObj(), S->getWorkCount());
                int C = MutateSol->getCritVal();
                if (C < Crit) {
                    delete Best;
                    Best = MutateSol->cpObj();
                    Crit = C;
                    Stop = 0;
                } else {
                    int D = C - S->getCritVal();
                    if (D <= 0) {
                        delete S;
                        S = MutateSol->cpObj();
                    } else {
                        double P = (double)std::rand() / RAND_MAX;
                        if (P < exp(-D / T->getTemp())) {
                            delete S;
                            S = MutateSol->cpObj();
                        }
                    }
                }
                delete MutateSol;
            }
            T->nxtIt();
        }
        delete S;
        return Best;
    }
};

#endif //TASK_2_SIMULATION_H
