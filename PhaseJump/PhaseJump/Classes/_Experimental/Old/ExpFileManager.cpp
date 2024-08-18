#include "FileManager.h"
#include <filesystem>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace PJ;

// FileManager    *pjFileManager = NULL;
//
// FileManager* FileManager::GetInstance()
//{
//     if (NULL == pjFileManager) {
//         PJ_ClassPtr c;
//         if (PJ_Class::FindClass(CLASS_FILE_MANAGER, c)) {
//             pjFileManager = static_cast<FileManager*>(c->FactoryNew(""));
//         }
//     }
//
//     return pjFileManager;
// }
//
// void FileManager::CollectDirSubPaths(String path, PJ_TStdVector<String>&
// subPaths)
//{
//     FileManager* fm = FileManager::GetInstance();
//     PJ_TStdVector<String>    contents;
//     CollectDirContents(path, contents);
//
//     subPaths.RemoveAll();
//     for (auto& _i : contents) {
//         subPaths.Add(fm->AppendPathStrings(path, _i));
//     }
// }
//

//
// String FileManager::StripPathDelim(String path) {
//    String result = path;
//    if (!result.empty() && result[result.size()-1] == GetPathDelim()) {
//        // Get rid of the ending path delimiter
//        result.erase(result.size()-1, 1);
//    }
//    return path;
//}
//
// void FileManager::MapSubpath(Path pathType, PathDomain domain, String
// subpath) {
//    SubpathMap::iterator i = mSubpathMap.find(pathType);
//    if (i == mSubpathMap.end()) {
//        mSubpathMap.insert(make_pair(pathType, DomainPathMap()));
//        i = mSubpathMap.find(pathType);
//    }
//    i->second[domain] = subpath;
//}
//
// String FileManager::GetPath(PJ_ConstStr &_path, Path pathType, PathDomain
// domain) {
//
//    FileManager* fm = FileManager::GetInstance();
//    String path = _path;
//
//    // Prepend the subpath BEFORE the path.
//    // Example: library + /user/ + "filename.xml"
//    SubpathMap::iterator i = mSubpathMap.find(pathType);
//    if (i != mSubpathMap.end()) {
//        DomainPathMap::iterator j = i->second.find(domain);
//        if (j != i->second.end()) {
//            String subpath = j->second;
//            path = fm->AppendPathStrings(subpath, path);
//        }
//    }
//
//    String result = getPath(path, pathType);
//
//    return result;
//
//}
//
// String FileManager::ApplicationPath(String path)
//{
//    return this->GetPath(path, FileManager::Path::PathApp,
//    FileManager::PathDomain::Default);
//
//}
//
// String FileManager::ApplicationDataPath(String path, FileManager::PathDomain
// domain)
//{
//    return this->GetPath(path, FileManager::Path::PathLibrary, domain);
//}
//
// String FileManager::UserDataPath(String path, FileManager::PathDomain domain)
// {
//    return this->GetPath(path, FileManager::Path::PathDocuments, domain);
//}
//
// String FileManager::TempFilesPath(String path) {
//    return this->GetPath(path, FileManager::Path::PathTemp,
//    FileManager::PathDomain::Default);
////}
//
// bool FileManager::IsImageFileType(String fileType) {
//    return (fileType.CompareNoCase("jpg") || fileType.CompareNoCase("png") ||
//    fileType.CompareNoCase("jpeg"));
//}
//
// bool FileManager::IsImageFile(String path) {
//    return IsImageFileType(FileType(path));
//}
//
// #pragma mark - Utilities
//
///*
//    InspectContentType
//
//    Supports the file type being embedded inside the file instead of via
//    extension.
//
//    CURRENTLY SUPPORTS:
//    1. XML tag: <file_type>, first line of file (no encoding
//    information/UTF-XXX).
//
// */
//// FUTURE: support non-XML format inspection if needed.
//// UNUSED: KEEP FOR REFERENCE.
////String FileManager::InspectContentType(String path, String reqExtension)
////{
////    String result = FileType(path);
////    if (reqExtension.empty() || result.CompareNoCase(reqExtension)) {
////        // Inspect the XML header to find out what type of content it is.
////        PJ_FilePtr file = PJ_File::NewFilePtr(path);
////        iostream* s = file->OpenForRead();
////        if (NULL != s) {
////            String str;
////            getline(*s, str);
////            if (str.empty() || str[0] != '<') {
////                return result;
////            }
////
////            String tagStr;
////            for (String::size_type i = 1; i < str.size(); i++) {
////                char theChar = str.at(i);
////                if ('>' == theChar) {
////                    break;
////                }
////
////                tagStr.append(1, theChar);
////            }
////
////            if (!tagStr.empty()) {
////                result = tagStr;
////                PJLog("INSPECTED XML FILE TYPE: %s", tagStr.c_str());
////            }
////        }
////    }
////
////    return result;
////
////}
//
// String FileManager::AppendPathStrings(String path1, String path2)
//{
//    if (path1.empty()) { return path2; }
//    if (path2.empty()) { return path1; }
//
//    String separatorStr(PathSeparatorString());
//    bool path1HasDirSuffix = path1.EndsWith(separatorStr);
//    bool path2HasDirPrefix = path2.StartsWith(separatorStr);
//
//    // Default: simple append.
//    String result;
//
//    // If both path substrings have a directory symbol, eliminate one.
//    if (path1HasDirSuffix && path2HasDirPrefix) {
//        result = path1.substr(0, path1.size() - 1) + path2;
//    }
//    else if (!path1HasDirSuffix &&!path2HasDirPrefix) {
//        result = path1 + separatorStr + path2;
//    }
//    else {
//        // Default: simple append.
//        result = path1 + path2;
//    }
//
//    return result;
//
//}
//
// String FileManager::StripPathSuffix(String path, char extendChar, bool
// stripExtendChar)
//{
//    String result = path;
//
//    bool foundSuffix = false;
//
//    // Strip out the app name to get the root path.
//    int i = static_cast<int>(result.size()) - 1;
//    for (; i >= 0; i--) {
//        if (extendChar == result.at(i)) {
//            foundSuffix = true;
//            break;
//        }
//    }
//
//    if (foundSuffix) {
//        if (!stripExtendChar) {
//            i++;    // Erase past / character.
//        }
//        result.erase(i, result.size() - i);
//    }
//
//    return result;
//}
//
// String FileManager::StripPathSegment(String path) {
//
//    char separatorChar = PathSeparatorChar();
//    return StripPathSuffix(path, separatorChar, true);
//}

///*
//    SwapFileType
//
//    Change the file extension of the file in the path.
//
// */
// String FileManager::SwapFileType(String path, String newType)
//{
//    char separatorChar = PathSeparatorChar();
//
//    String checkExtension = GetFileName(path, true);
//    String rn, rp;
//    rp = StripPathSuffix(path, separatorChar, true);    // Strip file name
//
//    if (!checkExtension.Contains(".")) {
//        // No extension. Append it.
//        rn = checkExtension + "." + newType;
//    }
//    else {
//        rn = GetFileName(path, false);    // Get root file name.
//        rn.append(".");
//        rn.append(newType);
//    }
//
//    return AppendPathStrings(rp, rn);
//
//}
//
// #pragma mark - UNIT TESTS
//
// #ifdef __UNIT_TESTS__
//
// #include "gtest.h"
//
//// FUTURE: add more unit tests.
// TEST(FileManager, Unit_Tests) {
//     FileManager *fm = FileManager::GetInstance();
//
//     {String path1("path1");
//         String path2("file.type");
//         String fullPath = fm->AppendPathStrings(path1, path2);
//
//         String expectStr = path1 + fm->PathSeparatorString() + path2;
//         EXPECT_STREQ(expectStr.c_str(), fullPath.c_str());
//
//         String fileType = fm->FileType(fullPath);
//         String fileNameFull = fm->GetFileName(fullPath, true);
//         String fileNamePartial = fm->GetFileName(fullPath, false);
//         EXPECT_STREQ("type", fileType.c_str());
//         EXPECT_STREQ("file.type", fileNameFull.c_str());
//         EXPECT_STREQ("file", fileNamePartial.c_str());
//
//         String swapPath = fm->SwapFileType(fullPath, "hello");
//         fileType = fm->FileType(swapPath);
//         EXPECT_STREQ("hello", fileType.c_str());
//
//         String stripPath = fm->StripPathSegment(fullPath);
//         expectStr = path1;
//         EXPECT_STREQ(expectStr.c_str(), stripPath.c_str());
//
//         // NOTE: not cross-platform compatible.
// #ifdef __NS_PHASE__
//         stripPath = fm->StripPathSuffix(fullPath, '.', false);
//         EXPECT_STREQ("path1/file.", stripPath.c_str());
//         stripPath = fm->StripPathSuffix(fullPath, '.', true);
//         EXPECT_STREQ("path1/file", stripPath.c_str());
// #endif
//     }
//
//     { String path1("test/path1");
//         path1 = fm->SwapFileType(path1, "append");
//         EXPECT_STREQ("test/path1.append", path1.c_str());
//     }
// }
//
// #endif
//
// #endif
