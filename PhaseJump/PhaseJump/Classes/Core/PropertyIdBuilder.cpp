#include "PropertyIdBuilder.h"

using namespace std;
using namespace PJ;

template <>
PropertyIdBuilder& PropertyIdBuilder::AddValue(String value) {
    ss << '"' << value << '"';
    return *this;
}

template <>
PropertyIdBuilder& PropertyIdBuilder::AddValue(const char* value) {
    return AddValue(String(value));
}
