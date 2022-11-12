#ifndef PJFILEPROCESSOR_UTILITY_H
#define PJFILEPROCESSOR_UTILITY_H

#include "FilesProcessor.h"
#include "_String.h"
#include <filesystem>

namespace PJ {
    namespace FileProcessor {
        namespace Utility {
            class RenameCSToHFileProcessor : public SomeFileProcessor {
                void Process(FilePath path) override {
                    std::error_code error;
                    std::filesystem::rename(path, String(path).ReplacingSuffix(".cs", ".h"));//, error);
                }
            };
        }
    }
}

#endif
