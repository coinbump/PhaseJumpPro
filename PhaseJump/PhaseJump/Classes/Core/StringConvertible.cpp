#include "StringConvertible.h"

using namespace std;
using namespace PJ;

ostream& PJ::operator<<(ostream& os, StringConvertible const& value) {
    return operator<<(os, value.ToString());
}
