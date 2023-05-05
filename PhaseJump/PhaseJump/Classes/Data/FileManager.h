#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Core.h"
#include "FilePath.h"
#include "VectorList.h"
#include <map>
#include <filesystem>

namespace PJ {
    class SomeFileManager : public Core {
    public:
        virtual bool IsDirectory(FilePath path) const = 0;

        virtual String FileExtension(FilePath path) const = 0;
        virtual String FileName(FilePath path, bool includeExtension) const = 0;

        virtual VectorList<FilePath> PathList(FilePath path, bool isRecursive) = 0;
    };

    /// Mock file manager for unit tests
    class MockFileManager : SomeFileManager {
        VectorList<FilePath> pathList;

        VectorList<FilePath> PathList(FilePath path, bool isRecursive) override { return pathList; }
    };

    /// Platform independent file manager
    class FileManager : public SomeFileManager
    {
    public:
        virtual bool IsDirectory(FilePath path) const override;

        String PathSeparatorString() const;
        char PathSeparatorChar() const { return std::filesystem::path::preferred_separator; }

        String FileExtension(FilePath path) const override;
        String FileName(FilePath path, bool includeExtension) const override;

        /// Create directories at path (folder is created at the final path segment)
        void CreateDirectories(FilePath path) const;

        VectorList<FilePath> PathList(FilePath path, bool isRecursive) override;
    };
}

#endif
