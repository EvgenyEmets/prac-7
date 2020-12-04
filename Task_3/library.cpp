#include "library.h"

std::string PolynomialFunc::ToString () {
    std::string Res;
    if (Coef[0] != 0) Res += std::to_string(Coef[0]);
    if (Coef.size() >= 2 && Coef[1] != 0) {
        Coef[1] < 0 ? Res += std::string(" - ") + std::to_string(-Coef[1]) :
                Res += std::string(" + ") + std::to_string(Coef[1]);
        Res += std::string("*x");
    }
    for (int i = 2; i < Coef.size(); i++) {
        if (Coef[i] == 0) {
            continue;
        } else {
            Coef[i] > 0 ? Res += std::string(" + ") + std::to_string(Coef[i]) :
                    Res += std::string(" - ") + std::to_string(-Coef[i]);
            Res += std::string("*x^") + std::to_string(i);
        }
    }
    return Res;
}

TFunctionPtr FuncFactory::Create(Functions Func, const std::initializer_list<double> &In) {
    switch (Func) {
        case IDENT:
            break;
        case CONST:
            break;
        case POWER:
            break;
        case EXP:
            break;
        case POLYNOMIAL:
            return TFunctionPtr(new PolynomialFunc(In));
    }
}

TFunctionPtr FuncFactory::Create(Functions Func, double In) {
    switch (Func) {
        case IDENT:
            return TFunctionPtr(new IdentFunc());
        case CONST:
            return TFunctionPtr(new ConstFunc(In));
        case POWER:
            return TFunctionPtr(new PowerFunc(In));
        case EXP:
            return TFunctionPtr(new ExpFunc(In));
        case POLYNOMIAL:
            break;
    }
}

double Gradient (TFunctionPtr Fun, int Iteration, double Eps, double Len) {
    double Point = 1;
    for (int i = 0; i < Iteration; i++) {
        if (Fun(Point)  > -Eps && Fun(Point) < Eps) {
            std::cout << Point << " is the root of " << Fun.ToString() << " with accuracy " << Eps << std::endl;
            return Point;
        }
        double Grad = Fun.GetDeriv(Point);
        Grad < 0 ? Point += Len : Point -= Len;
    }
    std::cout << "Cant found root" << std::endl;
    return Point;
}