#include "csv_mod.h"
#include "iostream"

int main() {
    int NProc, NWork, TimeLim1, TimeLim2;
    std::cout << "Process: ";
    std::cin >> NProc;
    std::cout << std::endl << "Works: ";
    std::cin >> NWork;
    std::cout << std::endl << "Time Limit 1: ";
    std::cin >> TimeLim1;
    std::cout << std::endl << "Time Limit 2: ";
    std::cin >> TimeLim2;
    generate(NWork, NProc, TimeLim1, TimeLim2);
    return 0;
}
