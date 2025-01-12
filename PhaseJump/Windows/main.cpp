#include <iostream>
#include "../PhaseJump/Classes/Core/AlignFunc.h"

using namespace PJ;

int main() {
    auto test = AlignFuncs::left;
    std::cout << test(10, 5) << std::endl;

    std::cout << "Hello from CMake!" << std::endl;
    return 0;
}
