#include "_String.h"
#include "VectorList.h"
#include <algorithm>

using namespace std;
using namespace PJ;

String String::ToLower() const {
    string result(*this);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);

    return result;
}

String String::ToUpper() const {
    string result(*this);
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);

    return result;
}

String String::Prefix(size_t size) const {
    if (size > this->size()) {
        return String();
    }

    return substr(0, size);
}

String String::Suffix(size_t size) const {
    if (size > this->size()) {
        return String();
    }

    return substr(this->size() - size, size);
}

VectorList<String> String::ComponentsSeparatedBy(char delimiter) {
    VectorList<String> result;
    Split(*this, delimiter, std::back_inserter(result));
    return result;
}
