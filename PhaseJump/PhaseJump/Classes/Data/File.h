#pragma once

#include "FilePath.h"
#include <fstream>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/22/24
 */
namespace PJ {
    /// Wraps file stream and file path
    class File {
    protected:
        std::ifstream file;

    public:
        FilePath path;

        /// Opens the document (if there is an existing file path)
        void Open();

        /// Opens the document with the specified file path
        void Open(FilePath path);

        /// Closes the document
        void Close();

        /// @return Returns true if the document is open
        bool IsOpen() const;

        // FUTURE: void Rename(String value);
    };
} // namespace PJ
