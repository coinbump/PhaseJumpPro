#pragma once

#include "FilePath.h"
#include "SomeProcessor.h"
#include "Updatable.h"
#include "Utils.h"
#include "VectorList.h"
#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/19/24
 */
namespace PJ {
    /// Processes a single file
    class FileProcessor : public SomeProcessor<FilePath> {
    public:
        using ProcessFileFunc = std::function<void(FilePath)>;

        ProcessFileFunc processFileFunc;

        FileProcessor() :
            SomeProcessor<FilePath>() {}

        virtual ~FileProcessor() {}

        virtual void Process(FilePath path) {
            GUARD(processFileFunc)
            processFileFunc(path);
        }

        // MARK: SomeProcessor

        size_t InputCount() const override {
            return 1;
        }

        size_t ProcessedCount() const override {
            return 0;
        }
    };

    enum class FileSearchType { Directory, Recursive };

    /// Processes a list of file paths (Example: rename files)
    class FilesProcessor : public SomeProcessor<VectorList<FilePath>> {
    public:
        using Base = SomeProcessor<VectorList<FilePath>>;
        using This = FilesProcessor;
        using ProcessFileFunc = std::function<void(FilePath)>;

        struct Settings {
            FileSearchType fileSearchType;

            Settings(FileSearchType fileSearchType) :
                fileSearchType(fileSearchType) {}
        };

    protected:
        Settings settings;
        size_t processedCount{};

    public:
        ProcessFileFunc processFileFunc;

        FilesProcessor(ProcessFileFunc processFileFunc, Settings settings) :
            processFileFunc(processFileFunc),
            settings(settings) {}

        virtual ~FilesProcessor() {}

        bool IsRecursive() const {
            return settings.fileSearchType == FileSearchType::Recursive;
        }

        int64_t UnfinishedCount() const {
            return input.size() - processedCount;
        }

        virtual void Scan(FilePath path) {
            if (fs::is_directory(path)) {
                if (IsRecursive()) {
                    for (auto& path : fs::recursive_directory_iterator{ path }) {
                        AddFile(path);
                    }
                } else {
                    for (auto& path : fs::directory_iterator{ path }) {
                        AddFile(path);
                    }
                }
            } else {
                AddFile(path);
            }
        }

        virtual void AddFile(FilePath path) {
            Add(input, path);
        }

        virtual void Process(FilePath path) {
            GUARD(processFileFunc)
            processFileFunc(path);
        }

        virtual FinishType ProcessNext() {
            if (processedCount >= 0 && processedCount < input.size()) {
                Process(input[processedCount]);
                processedCount++;
            }

            return processedCount < input.size() ? FinishType::Continue : FinishType::Finish;
        }

        // MARK: SomeProcessor

        virtual size_t InputCount() const {
            return input.size();
        }

        virtual size_t ProcessedCount() const {
            return processedCount;
        }
    };
} // namespace PJ
