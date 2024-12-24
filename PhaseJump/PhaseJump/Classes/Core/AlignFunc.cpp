#include "AlignFunc.h"

using namespace std;
using namespace PJ;

AlignFunc AlignFuncs::left = [](float layoutSize, float frameSize) { return 0; };

AlignFunc AlignFuncs::center = [](float layoutSize, float frameSize) {
    return layoutSize / 2.0f - frameSize / 2.0f;
};

AlignFunc AlignFuncs::right = [](float layoutSize, float frameSize) {
    return layoutSize - frameSize;
};
