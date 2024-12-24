#include "FileManager.h"
#include <filesystem>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace PJ;
namespace fs = std::filesystem;

bool FileManager::IsDirectory(FilePath filePath) {
    std::error_code errorCode;
    return filesystem::is_directory(filePath, errorCode);
}

String FileManager::FileExtension(FilePath filePath, bool withDot) {
    auto result = String(filePath.extension().string());

    if (!withDot) {
        if (!IsEmpty(result) && '.' == result[0]) {
            result = Suffix(result, result.size() - 1);
        }
    }

    return result;
}

String FileManager::FileName(FilePath filePath, bool includeExtension) {
    return includeExtension ? String(filePath.filename().string())
                            : String(filePath.stem().string());
}

void FileManager::CreateDirectories(FilePath filePath) {
    std::error_code errorCode;
    filesystem::create_directories(filePath, errorCode);
}

VectorList<FilePath> FileManager::PathList(FilePath path, FileSearchType searchType) {
    // Careful: avoid using this for large file lists on the main thread
    VectorList<FilePath> result;

    if (!IsDirectory(path)) {
        Add(result, path);
        return result;
    }

    if (searchType == FileSearchType::Recursive) {
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
