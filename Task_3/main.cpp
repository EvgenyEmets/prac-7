#include "library.h"

int main() {
    std::vector<TFunctionPtr> cont;
    FuncFactory FF;
    auto f = FF.Create(POWER, 2);
    cont.push_back(f);
    auto g = FF.Create(POLYNOMIAL, {7, 0, 3, 15});
    cont.push_back(g);
    for (auto ptr : cont) {
        std::cout << ptr.ToString() << " for x = 10 is " << ptr(10) << std::endl;
    }
    auto p = f + g;
    cont.push_back(p);
    std::cout << p.GetDeriv(1) << std::endl; // 15
    //auto h = f + "abc"; // std::logic_error
    std::cout << f.GetDeriv(3) << std::endl; // 6
    Gradient(f);
    for (auto ptr : cont) {
        ptr.Destroy();
    }
    cont.clear();
    return 0;
}
