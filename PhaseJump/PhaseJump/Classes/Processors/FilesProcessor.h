#ifndef PJFILESPROCESSOR_H
#define PJFILESPROCESSOR_H

#include "FilePath.h"
#include "SomeProcessor.h"
#include "Utils.h"
#include "VectorList.h"
#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    /// Process a single file
    class SomeFileProcessor : public SomeProcessor<FilePath> {
    public:
        SomeFileProcessor() :
            SomeProcessor<FilePath>(1) {}

        virtual ~SomeFileProcessor() {}

        virtual void Process(FilePath path) = 0;
    };

    enum class FileSearchType { Directory, Recursive };

    /// Process a list of file paths (Example: rename files)
    class FilesProcessor : public SomeProcessor<VectorList<FilePath>> {
    public:
        struct Settings {
            FileSearchType fileSearchType;

            Settings(FileSearchType fileSearchType) :
                fileSearchType(fileSearchType) {}
        };

    protected:
        UP<SomeFileProcessor> fileProcessor;
        Settings settings;

    public:
        using Base = SomeProcessor<VectorList<FilePath>>;

        FilesProcessor(Int filesCount, UP<SomeFileProcessor>& fileProcessor, Settings settings) :
            Base(filesCount),
            fileProcessor(std::move(fileProcessor)),
            settings(settings) {}

        virtual ~FilesProcessor() {}

        bool IsRecursive() const {
            return settings.fileSearchType == FileSearchType::Recursive;
        }

        virtual void Provide(FilePath path) {
            if (fs::is_directory(path)) {
                if (IsRecursive()) {
                    for (auto& path : fs::recursive_directory_iterator{ path }) {
                        ProvideFile(path);
                    }
                } else {
                    for (auto& path : fs::directory_iterator{ path }) {
                        ProvideFile(path);
                    }
                }
            } else {
                ProvideFile(path);
            }
        }

        virtual void ProvideFile(FilePath path) {
            input.Add(path);
            inputCount = std::max((Int)input.size(), inputCount);
        }

        virtual void Process(FilePath path) {
            if (!fileProcessor) {
                return;
            }
            fileProcessor->Process(path);
        }

        virtual bool ProcessNext() {
            if (processedInputCount >= 0 && processedInputCount < input.size()) {
                Process(input[processedInputCount]);
                processedInputCount++;
            }

            return processedInputCount < input.size();
        }
    };
} // namespace PJ

#endif
