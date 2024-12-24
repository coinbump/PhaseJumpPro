#include "Attributes.h"

using namespace std;
using namespace PJ;

UP<SomeAttribute> Attributes::NameAttribute(String name) {
    return NEW<Attribute<String>>(AttributeId::Name, name);
}
