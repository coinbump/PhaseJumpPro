#include <stdio.h>
#include <filesystem>
#include <iostream>

#include "FileManager.h"

using namespace std;
using namespace PJ;

bool FileManager::IsDirectory(FilePath filePath) const {
    std::error_code ec;
    return filesystem::is_directory(filePath, ec);
}

String FileManager::PathSeparatorString() const {
    return String(filesystem::path::preferred_separator);
}

String FileManager::FileExtension(FilePath filePath) const {
    return String(filePath.extension());
}

String FileManager::FileName(FilePath filePath, bool includeExtension) const {
    return includeExtension ? String(filePath.filename()) : String(filePath.stem());
}

void FileManager::CreateDirectories(FilePath filePath) const
{
    filesystem::create_directories(filePath);
}
