#pragma once

#include "FilePath.h"
#include "FileTypes.h"
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
    class FileProcessor : public InputProcessor<FilePath> {
    public:
        using ProcessFileFunc = std::function<void(FilePath)>;

        ProcessFileFunc processFileFunc;

        FileProcessor() :
            InputProcessor<FilePath>() {}

        virtual ~FileProcessor() {}

        virtual void Process(FilePath path) {
            GUARD(processFileFunc)
            processFileFunc(path);
        }

        // MARK: InputProcessor

        size_t InputCount() const override {
            return 1;
        }

        size_t ProcessedCount() const override {
            return 0;
        }
    };

    /// Processes a list of file paths (Example: rename files)
    class FilesProcessor : public InputProcessor<VectorList<FilePath>> {
    public:
        using Base = InputProcessor<VectorList<FilePath>>;
        using This = FilesProcessor;
        using ProcessFileFunc = std::function<void(FilePath)>;

        struct Config {
            FileSearchType fileSearchType;
        };

    protected:
        Config config;
        size_t processedCount{};

    public:
        ProcessFileFunc processFileFunc;

        FilesProcessor(ProcessFileFunc processFileFunc, Config const& config);

        virtual ~FilesProcessor() {}

        bool IsRecursive() const {
            return config.fileSearchType == FileSearchType::Recursive;
        }

        int64_t UnfinishedCount() const {
            return input.size() - processedCount;
        }

        /// Scans for files to be processed
        virtual void Scan(FilePath path);

        /// Adds a file to be processed
        virtual void AddFile(FilePath path);

        /// Processes the specified file
        virtual void Process(FilePath path);

        /// Processes the next file in the list
        virtual FinishType ProcessNext();

        // MARK: InputProcessor

        virtual size_t InputCount() const {
            return input.size();
        }

        virtual size_t ProcessedCount() const {
            return processedCount;
        }
    };
} // namespace PJ
