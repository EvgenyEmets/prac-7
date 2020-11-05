#ifndef TASK_2_MUTATION_H
#define TASK_2_MUTATION_H

#include "solution.h"
#include <ctime>
#include <random>

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

#endif //TASK_2_MUTATION_H
