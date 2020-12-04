#ifndef TASK_3_LIBRARY_H
#define TASK_3_LIBRARY_H

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <initializer_list>
#include <stdexcept>


enum Functions {
    IDENT,
    CONST,
    POWER,
    EXP,
    POLYNOMIAL
};

enum Operation {
    ADD,
    SUB,
    MUL,
    DIV
};

class TFunction {
public:
    virtual std::string ToString () = 0;
    virtual double GetDeriv(double Point) = 0;
    virtual double operator() (double Point) = 0;
    virtual ~TFunction() {}
};

class MathFunc : public TFunction {
    TFunction *Opperand1;
    TFunction *Opperand2;
    Operation Op;
public:
    MathFunc (TFunction *O1, TFunction *O2, Operation O) : Opperand1(O1), Opperand2(O2), Op(O) {}
    std::string ToString () override {
        std::string Res = Opperand1->ToString();
        switch (Op) {
            case ADD:
                Res += std::string(" + ");
                break;
            case SUB:
                Res += std::string(" - ");
                break;
            case MUL:
                Res += std::string(" * ");
                break;
            case DIV:
                Res += std::string(" / ");
                break;
        }
        Res += Opperand2->ToString();
        return Res;
    }
    double GetDeriv(double Point) override {
        double Res = 0;
        switch (Op) {
            case ADD:
                Res += Opperand1->GetDeriv(Point) + Opperand2->GetDeriv(Point);
                break;
            case SUB:
                Res += Opperand1->GetDeriv(Point) + Opperand2->GetDeriv(Point);
                break;
            case MUL:
                Res += Opperand1->GetDeriv(Point) * Opperand2->operator()(Point)
                       + Opperand2->GetDeriv(Point) * Opperand1->operator()(Point);
                break;
            case DIV:
                Res += (Opperand1->GetDeriv(Point) * Opperand2->operator()(Point)
                        - Opperand2->GetDeriv(Point) * Opperand1->operator()(Point))
                       / pow(Opperand2->operator()(Point), 2);
                break;
        }
        return Res;
    }
    double operator() (double Point) override {
        double Res = 0;
        switch (Op) {
            case ADD:
                Res += Opperand1->operator()(Point) + Opperand2->operator()(Point);
                break;
            case SUB:
                Res += Opperand1->operator()(Point) - Opperand2->operator()(Point);
                break;
            case MUL:
                Res += Opperand1->operator()(Point) * Opperand2->operator()(Point);
                break;
            case DIV:
                Res += Opperand1->operator()(Point) / Opperand2->operator()(Point);
                break;
        }
        return Res;
    }
};

class IdentFunc : public TFunction {
public:
    std::string ToString () override {
        return std::string("x");
    }
    double GetDeriv(double Point) override {
        return 1;
    }
    double operator() (double Point) override {
        return Point;
    }
};

class ConstFunc : public TFunction {
    double Value;
public:
    ConstFunc (double In) : Value(In) {}
    std::string ToString () override {
        return std::to_string(Value);
    }
    double GetDeriv(double Point) override {
        return 0;
    }
    double operator() (double Point) override {
        return Value;
    }
};

class PowerFunc : public TFunction {
    double Exp;
public:
    PowerFunc (double In) : Exp(In) {}
    std::string ToString () override {
        return std::string("x^") + std::to_string(Exp);
    }
    double GetDeriv(double Point) override {
        return Exp * pow(Point, Exp - 1);
    }
    double operator() (double Point) override {
        return pow(Point, Exp);
    }
};

class ExpFunc : public TFunction {
    double Base;
public:
    ExpFunc (double In) : Base(In) {}
    std::string ToString () override {
        return std::to_string(Base) + std::string("^x");
    }
    double GetDeriv(double Point) override {
        return log(Base) * pow(Base, Point);
    }
    double operator() (double Point) override {
        return pow(Base, Point);
    }
};

class  PolynomialFunc : public TFunction {
    std::vector<double> Coef;
public:
    PolynomialFunc (std::initializer_list<double> In) : Coef(In) {}
    std::string ToString () override;
    double GetDeriv(double Point) override {
        double Res = 0;
        for (int i = 1; i < Coef.size(); i++){
            Res += Coef[i] * i * pow(Point, i - 1);
        }
        return Res;
    }
    double operator() (double Point) override {
        double Res = 0;
        for (int i = 0; i < Coef.size(); i++){
            Res += Coef[i] * pow(Point, i);
        }
        return Res;
    }
};

class TFunctionPtr {
    TFunction *Obj;
public:
    TFunctionPtr (TFunction *In) : Obj(In) {}
    std::string ToString () {
        return Obj->ToString();
    }
    double GetDeriv(double Point) {
        return Obj->GetDeriv(Point);
    }
    double operator() (double Point) {
        return Obj->operator()(Point);
    }
    TFunctionPtr operator+ (TFunctionPtr &Op) {
        return TFunctionPtr(new MathFunc(this->Obj, Op.Obj, ADD));
    }
    TFunctionPtr operator- (TFunctionPtr Op) {
        return TFunctionPtr(new MathFunc(this->Obj, Op.Obj, SUB));
    }
    TFunctionPtr operator* (TFunctionPtr Op) {
        return TFunctionPtr(new MathFunc(this->Obj, Op.Obj, MUL));
    }
    TFunctionPtr operator/ (TFunctionPtr Op) {
        return TFunctionPtr(new MathFunc(this->Obj, Op.Obj, DIV));
    }
    template <typename T>
    TFunctionPtr operator+ (T &Op) {
        throw std::logic_error("std::logic_error");
    }
    template <typename T>
    TFunctionPtr operator- (T Op) {
        throw std::logic_error("std::logic_error");
    }
    template <typename T>
    TFunctionPtr operator* (T Op) {
        throw std::logic_error("std::logic_error");
    }
    template <typename T>
    TFunctionPtr operator/ (T Op) {
        throw std::logic_error("std::logic_error");
    }
    void Destroy() {
        //std::cout << "DELETE: " << Obj->ToString() << std::endl;
        delete (Obj);
    }
};

class FuncFactory {
public:
    TFunctionPtr Create(Functions Func, const std::initializer_list<double> &In);
    TFunctionPtr Create(Functions Func, double In);
};

double Gradient (TFunctionPtr Fun, int Iteration = 10, double Eps = 0.1, double Len = 1);

#endif