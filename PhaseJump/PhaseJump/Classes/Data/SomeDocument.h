#pragma once

#include "FilePath.h"
#include "ResourceCatalog.h"
#include "Result.h"
#include "StringUtils.h"
#include "Utils.h"
#include "Void.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/26/24
 */
namespace PJ {
    /**
     Handles common behavior for documents

     A document may or may not be associated with a file
     */
    class SomeDocument {
    public:
        using This = SomeDocument;

        using Failure = std::exception;
        using Result = PJ::Result<Void, Failure>;

    protected:
        FilePath filePath;

        /// If true, the file is loaded
        bool isLoaded{};

        /// Counts the # of modifications. A count not a flag, so we can decrement on undo
        int modifyCount{};

    public:
        struct Config {
            String name;
            String description;
            FilePath filePath;
        };

        SomeDocument(Config const& config = {}) :
            name(config.name),
            description(config.name),
            filePath(config.filePath) {}

        virtual ~SomeDocument() {}

        FilePath GetFilePath() const {
            return filePath;
        }

        String name;
        String description;

        /// Resources for this document (flushed when document is closed)
        ResourceCatalog resources;

        /// @return Returns true if the document has been loaded
        bool IsLoaded() const {
            return isLoaded;
        }

        /// @return Returns true if the document has been modified
        bool IsModified() const {
            return modifyCount > 0;
        }

        /// Pushes the modify counter by 1, useful if we want undo to reset the modified state to
        /// false
        void ModifyPush() {
            modifyCount++;
        }

        /// Pops the modify counter by 1, useful if we want undo to reset the modified state to
        /// false
        void ModifyPop() {
            modifyCount--;
        }

        /// Loads the document
        virtual void Load() {}

        /// Saves the document
        virtual void Save() {}
    };

    /// Document with a core
    template <class Core = Void>
    class Document : public SomeDocument {
    public:
        using Base = SomeDocument;
        using This = Document;

        using DocumentFunc = std::function<void(This&)>;
        using ResultFunc = std::function<Result(This&)>;

        Core core{};

        struct Config {
            Core core{};

            String name;
            String description;
            FilePath filePath;
        };

        /// Called to load the document
        ResultFunc loadFunc;

        /// Called to save the document
        ResultFunc saveFunc;

        /// Called after the document is loaded
        DocumentFunc onLoadFunc;

        /// Called after the document is saved
        DocumentFunc onSaveFunc;

        Document(Config const& config = {}) :
            Base({ .name = config.name,
                   .description = config.description,
                   .filePath = config.filePath }),
            core(config.core) {}

        // MARK: SomeDocument

        void Load() override {
            GUARD(loadFunc)

            auto result = loadFunc(*this);
            GUARD(result.IsSuccess())

            isLoaded = true;

            GUARD(onLoadFunc)
            onLoadFunc(*this);
        }

        void Save() override {
            GUARD(saveFunc)

            auto result = saveFunc(*this);
            GUARD(result.IsSuccess())

            GUARD(onSaveFunc)
            onSaveFunc(*this);
        }
    };
} // namespace PJ
