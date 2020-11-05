#ifndef TASK_2_SOLUTION_H
#define TASK_2_SOLUTION_H

#include <vector>
#include <list>
#include <iostream>

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
#endif //TASK_2_SOLUTION_H
