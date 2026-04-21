#include "Dev.h"
#include "Macros.h"

using namespace std;
using namespace PJ;

#ifdef DEBUG
void AssertImpl(bool isTrue) {
    assert(isTrue);
}
#else
void AssertImpl(bool isTrue) {}
#endif

PJ::AssertFunc PJ::assertFunc = [](bool value) { AssertImpl(value); };
String PJ::logLevel = LogLevel::Quiet;

void PJ::Assert(bool isTrue) {
    GUARD(assertFunc)
    assertFunc(isTrue);
}

bool PJ::IsLogVerbose() {
    return logLevel == LogLevel::Verbose;
}
