#include "Attributes.h"

using namespace std;
using namespace PJ;

SP<SomeAttribute> Attributes::NameAttribute(String name) {
    return MAKE<Attribute<String>>(AttributeId::Name, name);
}
