#include "FileManager.h"
#include <filesystem>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace PJ;
namespace fs = std::filesystem;

bool FileManager::IsDirectory(FilePath filePath) const {
    std::error_code errorCode;
    return filesystem::is_directory(filePath, errorCode);
}

String FileManager::PathSeparatorString() const {
    return String(filesystem::path::preferred_separator);
}

String FileManager::FileExtension(FilePath filePath, bool withDot) const {
    auto result = String(filePath.extension().string());

    if (!withDot) {
        result.RemoveIf([](char c) { return c == '.'; });
    }

    return result;
}

String FileManager::FileName(FilePath filePath, bool includeExtension) const {
    return includeExtension ? String(filePath.filename().string())
                            : String(filePath.stem().string());
}

void FileManager::CreateDirectories(FilePath filePath) const {
    filesystem::create_directories(filePath);
}

// Important: always use this with threads or you will hang the app for large
// lists
VectorList<FilePath> FileManager::PathList(FilePath path, bool isRecursive) {
    VectorList<FilePath> result;

    if (!IsDirectory(path)) {
        Add(result, path);
        return result;
    }

    if (isRecursive) {
        for (auto& path : fs::recursive_directory_iterator{ path }) {
            Add(result, path);
        }
    } else {
        for (auto& path : fs::directory_iterator{ path }) {
            Add(result, path);
        }
    }

    return result;
}
