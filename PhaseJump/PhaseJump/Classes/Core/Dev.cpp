#include "Dev.h"

using namespace std;
using namespace PJ;

void Asserter::Assert(bool isTrue) {
    GUARD(assertFunc)
    assertFunc(isTrue);
}

#ifdef DEBUG
PJ::Asserter PJ::asserter([](bool isTrue) { assert(isTrue); });
#else
PJ::Asserter PJ::asserter([](bool isTrue) {});
#endif

void PJ::Assert(bool isTrue) {
    asserter.Assert(isTrue);
}
