#pragma once

#include "FilesProcessor.h"

using namespace PJ;

/*
 CODE REVIEW: /td
 */
namespace PJ_Dev {
    /**
     Scans for .h files and creates the Xcode framework umbrella header files
     */
    class UmbrellaHeadersFilesProcessor : public FilesProcessor {
    public:
        using This = UmbrellaHeadersFilesProcessor;
        using Base = FilesProcessor;

        UmbrellaHeadersFilesProcessor();

        // MARK: FilesProcessor

        void Process(FilePath filePath) override;
    };
} // namespace PJ_Dev
