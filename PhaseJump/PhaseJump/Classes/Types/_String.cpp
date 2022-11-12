#include "_String.h"
#include <iomanip>

using namespace std;
using namespace PJ;

String String::ToLower()
{
    string result(*this);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);

    return result;
}

String String::ToUpper()
{
    string result(*this);
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);

    return result;
}

String String::Prefix(size_t size) {
    if (size > this->size()) { return String(); }

    return substr(0, size);
}

String String::Suffix(size_t size) {
    if (size > this->size()) { return String(); }

    return substr(this->size() - size, size);
}
