#ifndef PJFILEPROCESSOR_UTILITY_H
#define PJFILEPROCESSOR_UTILITY_H

#include "FilesProcessor.h"
#include "StringUtils.h"
#include <filesystem>

// CODE REVIEW: ?/23
namespace PJ {
    namespace FileProcessor {
        namespace Utility {
            // TODO: this should just be a func
            class RenameCSToHFileProcessor : public SomeFileProcessor {
                void Process(FilePath path) override {
                    std::error_code error;
                    std::filesystem::rename(
                        path, ReplacingSuffix(String(path), ".cs", ".h")
                    ); //, error);
                }
            };
        } // namespace Utility
    } // namespace FileProcessor
} // namespace PJ

#endif
