#include "ThemeBuilder.h"

using namespace std;
using namespace PJ;

ThemeBuilder::ThemeBuilder(DesignSystem& designSystem, ViewBuilder& vb) :
    designSystem(designSystem),
    vb(vb) {}

ThemeBuilder::ThemeBuilder(ViewBuilder& vb) :
    designSystem(fallbackDesignSystem),
    vb(vb) {
    auto designSystem = vb.GetDesignSystem();
    if (designSystem != nullptr) {
        this->designSystem = *designSystem;
    }
}
