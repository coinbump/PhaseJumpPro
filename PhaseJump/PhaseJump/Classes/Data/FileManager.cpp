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

String FileManager::FileExtension(FilePath filePath, FileExtensionFormat extensionFormat) {
    auto result = String(filePath.extension().string());

    switch (extensionFormat) {
    case FileExtensionFormat::WithDot:
        return result;
    case FileExtensionFormat::NoDot:
        if (!IsEmpty(result) && '.' == result[0]) {
            result = Suffix(result, result.size() - 1);
        }
    }

    return result;
}

String FileManager::FileName(FilePath filePath, FileNameFormat nameFormat) {
    return nameFormat == FileNameFormat::WithExtension ? String(filePath.filename().string())
                                                       : String(filePath.stem().string());
}

String FileManager::ParentPath(FilePath path) {
    return path.parent_path();
}

void FileManager::CreateDirectories(FilePath filePath) {
    std::error_code errorCode;
    filesystem::create_directories(filePath, errorCode);
}

VectorList<FilePath> FileManager::FilePathList(FilePath path, FileSearchType searchType) {
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
