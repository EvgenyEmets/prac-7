#ifndef TASK_2_TEMPERATURE_H
#define TASK_2_TEMPERATURE_H

#include <cmath>

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

#endif //TASK_2_TEMPERATURE_H
