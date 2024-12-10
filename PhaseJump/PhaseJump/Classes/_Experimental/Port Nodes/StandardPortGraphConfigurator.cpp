#include "StandardPortGraphConfigurator.h"

using namespace std;
using namespace PJ;

SP<SomeAttribute> PortGraphAttributes::HidePortAttribute() {
    return MAKE<Attribute<Void>>(PortGraphAttributeId::NoPort);
}
