#include "Attributes.h"

using namespace std;
using namespace PJ;

UP<Attribute> Attributes::NameAttribute(String name) {
    return NEW<ValueAttribute<String>>(AttributeId::Name, name);
}
