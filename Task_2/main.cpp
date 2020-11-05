#include <cmath>
#include <vector>
#include <list>
#include <ctime>
#include <random>
#include <iostream>
#include <thread>

class Work {
    int num, time;
public:
    Work (int n, int t) : num(n), time(t) {}
    Work () : num(0), time(0) {}
    int getNum() {
        return num;
    }
    int getTime() {
        return time;
    }
};

//      ~~~SOLUTION & TARGET FUNCTION~~~
//      Solution
//      |
//      ---Shed_Len

class Solution {
public:
    virtual ~Solution() {}
    virtual int getCritVal() = 0;
    virtual Solution* cpObj() = 0;
    virtual int getWorkCount() = 0;
    virtual void printSol() = 0;
};

class Shed_Len : public Solution {
public:
    std::vector<std::list<Work>> S;
    int Works;

    Shed_Len() {
        S = std::vector<std::list<Work>> ();
        Works = 0;
    }
    Shed_Len(int N, int M, std::vector<Work> A) {
        Works = N;
        for (int i = 0; i < M; i++) {
            S.emplace_back(std::list<Work>());
        }
        for (int i = 0; i < N; i++) {
            S[0].push_back(A[i]);
        }
    }
    int getCritVal() override {
        int max = 0;
        for (int i = 0; i < S.size(); i++) {
            int tmp = 0;
            for (auto j = S[i].begin(); j != S[i].end(); j++) {
                tmp += j->getTime();
            }
            if (max < tmp) {
                max = tmp;
            }
        }
        return max;
    }
    Solution* cpObj() {
        return new Shed_Len(*this);
    }
    int getWorkCount() {
        return Works;
    }
    void printSol() {
        for (int i = 0; i < S.size(); i++) {
            std::cout << "proc" << i + 1 << std::endl;
            for (auto j = S[i].begin(); j != S[i].end(); j++) {
                std::cout << "Work " << j->getNum() << " Time " << j->getTime() << std::endl;
            }
        }
    }
};

//      ~~~MUTATION~~~
//      Mutation
//      |
//      -Mutation_R

class Mutation {
public:
    virtual Solution *Mutate(Solution *in, int N) = 0;
    virtual Mutation *cpObj() = 0;
};

class Mutation_R : public Mutation{
    int count;
public:
    Mutation_R () : count(0) {}
    Solution *Mutate(Solution *in, int N) {
        Shed_Len *cur = dynamic_cast<Shed_Len*> (in);
        if (cur == nullptr) {
            return nullptr;
        }
        std::srand(std::time(nullptr) + count);
        count++;
        int targWN = std::rand() % N, targPN = std::rand() % cur->S.size();
        //std::cout << targWN << " : " << targPN << std::endl;
        Work targW;
        bool flg = false;

        for (int i = 0; i < cur->S.size(); i++) {
            if (flg) {
                break;
            }
            for (auto j = cur->S[i].begin(); j != cur->S[i].end(); j++) {
                if (j->getNum() == targWN) {
                    targW = *j;
                    cur->S[i].erase(j);
                    cur->S[targPN].push_back(targW);
                    flg = !flg;
                    break;
                }
            }
        }
        return cur;
    }
    Mutation *cpObj() {
        return new Mutation_R(*this);
    }
};

//      ~~~TEMPERATURE CONTROL~~~
//      Abstract class Temperature
//      |
//      |--Boltzmann
//      |
//      |--Cauchy
//      |
//      ---Simple

class Temperature {
protected:
    double T;
    int iteration;
public:
    Temperature(double tT) : T(tT), iteration(1) {}
    virtual double getTemp() = 0;
    void nxtIt () {
        iteration++;
    }
    virtual Temperature* cpObj() = 0;
};

class Boltzmann : public Temperature {
public:
    Boltzmann (double tT) : Temperature(tT) {}
    double getTemp() override {
        return T / log(iteration + 1);
    }
    Temperature* cpObj() {
        return new Boltzmann(*this);
    }
};

class Cauchy : public Temperature {
public:
    Cauchy (double tT) : Temperature(tT) {}
    double getTemp() override {
        return T / (iteration + 1);
    }
    Temperature* cpObj() {
        return new Cauchy(*this);
    }
};

class Simple : public Temperature {
public:
    Simple (double tT) : Temperature(tT) {}
    double getTemp() override {
        return T * log(iteration + 1) / (iteration + 1);
    }
    Temperature* cpObj() {
        return new Simple(*this);
    }
};

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
        while (Stop < 100) {
            Stop++;
            for (int i = 0; i < 100; i++) {
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
                        std::srand(std::time(nullptr));
                        double P = (double)std::rand() / RAND_MAX;
                        if (P > exp(-D / T->getTemp())) {
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

void func (Shed_Len *SInit, Mutation_R *MInit, Boltzmann *TInit, Shed_Len **Res, int thr) {
    Shed_Len *S = dynamic_cast<Shed_Len*>(SInit->cpObj());
    Boltzmann *T = dynamic_cast<Boltzmann *>(TInit->cpObj());
    Mutation_R *M = dynamic_cast<Mutation_R *>(MInit->cpObj());
    SimAnn A(S, T, M);
    *Res = dynamic_cast<Shed_Len*>(A.Simulate());
    //(*Res)->printSol();
    delete T;
    delete M;
}

int main() {
    Work W1(1,1);
    Work W2(2,2);
    Work W3(3,2);
    Work W4(4,3);
    Work W5(5,5);
    Work W6(6,3);
    std::vector<Work> W;
    W.push_back(W1);
    W.push_back(W2);
    W.push_back(W3);
    W.push_back(W4);
    W.push_back(W5);
    W.push_back(W6);
    Shed_Len* S;
    S = new Shed_Len(6, 2, W);
    Mutation_R M;
    Boltzmann T(100);
    //SimAnn A(S, &T, &M);
    //Shed_Len *Ans = dynamic_cast<Shed_Len*>(A.Simulate());
    //Ans->printSol();
    //delete Ans;
    int NTread = 6;
    std::vector<std::thread> ThreadVector;
    for (int iter = 0; iter < 10; iter++) {
        std::vector<Shed_Len*> Buffer(NTread);
        for (int i = 0; i < NTread; i++) {
            //func(S, &M, &T, Buffer, i);
            ThreadVector.emplace_back(func, S, &M, &T, &(Buffer[i]), i);
        }
        for (int i = 0; i < NTread; i++) {
            ThreadVector[i].join();
        }
        delete S;
        //Buffer[0]->printSol();
        Shed_Len *BestSolution = dynamic_cast<Shed_Len*>(Buffer[0]->cpObj());
        int BestMetric = BestSolution->getCritVal();
        for (int i = 0; i < NTread; i++) {
            if (BestMetric > Buffer[i]->getCritVal()) {
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
    }
    S->printSol();
    std::cout << "PROGRAM END" << std::endl;
    delete S;

    return 0;
}