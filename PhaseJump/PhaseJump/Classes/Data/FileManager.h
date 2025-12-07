#pragma once

#include "FilePath.h"
#include "FileTypes.h"
#include "StandardCore.h"
#include "VectorList.h"
#include <filesystem>
#include <future>
#include <map>

/*
 RATING: 4 stars
 Has unit tests (partial)
 CODE REVIEW: 12/17/24
 */
namespace PJ {
    /// Interface to an object that manages files
    class SomeFileManager : public Base {
    public:
        enum class FileExtensionFormat {
            /// Includes dot in file extension (".mov")
            WithDot,

            /// No dot in file extension ("mov")
            NoDot
        };

        enum class FileNameFormat {
            /// Includes file extension ("file.mov")
            WithExtension,

            /// Removes file extension ("file")
            NoExtension
        };

        /// @return Returns true if the file at the specified path is a directory/folder
        virtual bool IsDirectory(FilePath path) = 0;

        /// @return Returns the file extension for a file path with or without the dot
        virtual String FileExtension(FilePath path, FileExtensionFormat extensionFormat) = 0;

        /// @return Returns the file name for a path with or without the file extension
        virtual String FileName(FilePath path, FileNameFormat nameFormat) = 0;

        /// @return Returns a list of file paths starting from the specified folder file path
        virtual VectorList<FilePath> FilePathList(FilePath path, FileSearchType searchType) = 0;

        /// @return Returns the parent path
        virtual String ParentPath(FilePath path) = 0;

        /// @return Returns the preferred path separator character
        virtual char PreferredSeparator() const = 0;

        /// (Experimental). Not sure how std::async benefits us yet
        std::future<VectorList<FilePath>>
        FilePathListAsync(FilePath path, FileSearchType searchType) {
            return std::async([=, this] { return FilePathList(path, searchType); });
        }

        /// Create directories at path (folder is created at the final path segment)
        virtual void CreateDirectories(FilePath path) = 0;

        /// @return Returns the path to store persistent user data like preferences, save files,
        /// etc.
        virtual FilePath PersistentDataPath(String companyName, String applicationName) = 0;
    };

    // MARK: - FileManager

    /// Platform independent file manager
    class FileManager : public SomeFileManager {
    public:
        // MARK: SomeFileManager

        void CreateDirectories(FilePath path) override;
        bool IsDirectory(FilePath path) override;
        String FileExtension(FilePath path, FileExtensionFormat extensionFormat) override;
        String FileName(FilePath path, FileNameFormat nameFormat) override;
        VectorList<FilePath> FilePathList(FilePath path, FileSearchType searchType) override;
        String ParentPath(FilePath path) override;

        char PreferredSeparator() const override {
            return std::filesystem::path::preferred_separator;
        }
    };
} // namespace PJ
