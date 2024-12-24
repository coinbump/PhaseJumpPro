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
 CODE REVIEW: 12/23/24
 */
namespace PJ {
    /**
     Handles common behavior for documents

     A document may or may not be associated with a file
     */
    class Document {
    public:
        using This = Document;

        using Failure = std::exception;
        using Result = PJ::Result<Void, Failure>;

        using DocumentFunc = std::function<void(This&)>;
        using ResultFunc = std::function<Result(This&)>;

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

        FilePath GetFilePath() const {
            return filePath;
        }

        Document(Config config = {}) :
            name(config.name),
            description(config.name),
            filePath(config.filePath) {}

        String name;
        String description;

        /// Called to load the document
        ResultFunc loadFunc;

        /// Called to save the document
        ResultFunc saveFunc;

        /// Called after the document is loaded
        DocumentFunc onLoadFunc;

        /// Called after the document is saved
        DocumentFunc onSaveFunc;

        /// Called after the document is closed
        DocumentFunc onCloseFunc;

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
        void Load();

        /// Saves the document
        void Save();

        /// Closes the document
        void Close();
    };
} // namespace PJ
