#ifndef PJFILESPROCESSOR_H
#define PJFILESPROCESSOR_H

#include "SomeProcessor.h"
#include "FilePath.h"
#include "VectorList.h"
#include "Macros.h"
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

/*
 RATING: 5 stars
 Reusable utility pattern with unit tests
 CODE REVIEW: 11/12/22
 */
namespace PJ {
    /// Proces a single file
    class SomeFileProcessor : public SomeProcessor<FilePath> {
    public:
        SomeFileProcessor() : SomeProcessor<FilePath>(1) {}
        virtual ~SomeFileProcessor() {}

        virtual void Process(FilePath path) = 0;
    };

    /// Process a list of file paths (Example: rename files)
    class FilesProcessor : public SomeProcessor<VectorList<FilePath>> {
    public:
        struct Settings {
            bool isRecursive;

            Settings(bool isRecursive = false)
            : isRecursive(isRecursive) {
            }
        };

    protected:
        SP<SomeFileProcessor> fileProcessor;
        Settings settings;

    public:
        using Base = SomeProcessor<VectorList<FilePath>>;

        FilesProcessor(Int filesCount, SP<SomeFileProcessor> fileProcessor, Settings settings = Settings())
        : Base(filesCount),
        fileProcessor(fileProcessor),
        settings(settings) {
        }
        virtual ~FilesProcessor() {}

        virtual void Provide(FilePath path) {
            if (fs::is_directory(path)) {
                if (settings.isRecursive) {
                    for (auto const& path : fs::recursive_directory_iterator { path })
                    {
                        ProvideFile(path);
                    }
                } else {
                    for (auto const& path : fs::directory_iterator { path })
                    {
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
            if (!fileProcessor) { return; }
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
}

#endif
