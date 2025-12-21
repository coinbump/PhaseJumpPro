#include "StandardPortGraphConfigurator.h"

using namespace std;
using namespace PJ;

SP<Attribute> PortGraphAttributes::HidePortAttribute() {
    return MAKE<ValueAttribute<Void>>(PortGraphAttributeId::NoPort);
}
