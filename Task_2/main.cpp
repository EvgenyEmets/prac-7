#include <cmath>


class Work {
    int num, time;
public:
    Work (int n, int t) : num(n), time(t) {}
    int getNum() {
        return num;
    }
    int getTime() {
        return time;
    }
};

class Solution {
protected:
    std::vector<std::list<Work>> S;
public:
    Solution(int N, int M, std::vector<Work> A) {
        for (int i = 0; i < M; i++) {
            S.push_back(std::list<Work>(1, Work(i + 1 + N, 0)));
        }
        for (int i = 0; i < a.size(); i++) {
            S[0].push_back(A[i]);
        }
    }
    virtual int getCritVal() = 0;
};

class Mutation {
public:
    virtual void Mutate() = 0;
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
};

class Boltzmann : public Temperature {
    Boltzmann (double tT) : Temperature(tT) {}
    double getTemp() {
        return T / log(++iteration);
    }
};

class Cauchy : public Temperature {
    Cauchy (double tT) : Temperature(tT) {}
    double getTemp() {
        return T / ++iteration;
    }
};

class Simple : public Temperature {
    Simple (double tT) : Temperature(tT) {}
    double getTemp() {
        int i = ++iteration;
        return T * log(i) / i;
    }
};