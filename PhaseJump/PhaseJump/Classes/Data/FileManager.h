#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Core.h"
#include "FilePath.h"
#include <map>
#include <filesystem>

namespace PJ {
    /**
     Platform independent file manager
     */
    class FileManager : public Core
    {
    public:
        virtual bool IsDirectory(FilePath path) const;

        String PathSeparatorString() const;
        char PathSeparatorChar() const { return std::filesystem::path::preferred_separator; }

        String FileExtension(FilePath path) const;
        String FileName(FilePath path, bool includeExtension) const;

        /// Create directories at path (folder is created at the final path segment)
        void CreateDirectories(FilePath path) const;
    };
}

#endif
