#pragma once

#include "SomeLoadResourcesOperation.h"

namespace PJ {
    /// Load a texture atlas .fnt file created by Bitmap Font Generator
    class LoadBitmapFontOperation : public SomeLoadResourcesOperation {
    public:
        using Base = SomeLoadResourcesOperation;

        LoadBitmapFontOperation(ResourceInfo info, LoadResourcesModel& loadResourcesModel) :
            Base(info, loadResourcesModel) {}

        // MARK: SomeLoadResourcesOperation

        Result LoadResources() override;
    };
} // namespace PJ
