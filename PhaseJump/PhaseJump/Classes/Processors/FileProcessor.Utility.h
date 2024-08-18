#ifndef PJFILEPROCESSOR_UTILITY_H
#define PJFILEPROCESSOR_UTILITY_H

#include "_String.h"
#include "FilesProcessor.h"
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
                        path, String(path).ReplacingSuffix(".cs", ".h")
                    ); //, error);
                }
            };
        } // namespace Utility
    } // namespace FileProcessor
} // namespace PJ

#endif
