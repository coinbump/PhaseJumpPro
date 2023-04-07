#ifndef EXPFILEMANAGER_H
#define EXPFILEMANAGER_H

#include "_String.h"
#include "Array.h"
#include "Core.h"
#include <map>
#include <filesystem>
#include <iostream>

namespace PJ {
    /*
     FileManager

     Platform-independent abstract base for a file manager.

     */
    // FUTURE: use more stl functions for portability.
    // FUTURE: support more platforms (Windows?)
    // FUTURE: unify with PJ_FileUtils
    class ExpFileManager : public Core
    {
    public:
//
//
//
//        enum class Path {
//            PathLibrary,    // Stored permanently, not visible to users
//            PathDocuments,    // Stored permanently, visible to users
//            FrameworkResources,
//            AppResources,    // Application resources
//            PathTemp,        // Temporary files, may be deleted.
//            PathApp        // Application path
//        };
//
//        /*
//         PathDomain
//
//         Specifies domain of path. Used for specifying subpaths.
//
//         EXAMPLE: Multiple user profiles, each stores files in a separate subpath of the main path.
//
//         */
//        enum class PathDomain {
//            Default = 0,
//            User    // Subpath for current user profile
//        };
//
//    protected:
//        typedef std::map<PathDomain, String>    DomainPathMap;
//        typedef std::map<Path, DomainPathMap>    SubpathMap;
//        SubpathMap    mSubpathMap;    // Maps app-specified subpaths for general paths
//
//        virtual String getPath(String path, Path pathType) = 0;
//
//    public:
//        static FileManager* GetInstance();
//
//
//
//
//
//        virtual char PathSeparatorChar() const = 0;
//
//        virtual bool FileExistsAtPath(String path) = 0;
//        virtual void CollectDirContents(String path, Array<String>& dirs) = 0;
//        virtual void CollectDirSubPaths(String path, Array<String>& subPaths);
//        virtual void NewDirectory(String path) = 0;
//        virtual bool DeleteFile(String path) = 0;
//        virtual bool RenameFile(String first, String final) = 0;
//        virtual void CreateIntermediateDirs(String path) = 0;
//        virtual bool CopyFileTo(String source, String dest, bool replace) = 0;
//
//        virtual bool IsImageFileType(String fileType);
//        virtual bool IsImageFile(String path);
//
//        virtual char GetPathDelim() const { return PathSeparatorChar(); }
//        String StripPathDelim(String path);
//
//        void MapSubpath(Path pathType, PathDomain domain, String subpath);
//
//        String GetPath(String path, Path pathType, PathDomain domain = FileManager::PathDomain::Default);
//
//        virtual String AppendPathStrings(String path1, String path2);
//        virtual String StripPathSuffix(String path, char extendChar, bool stripExtendChar);
//        virtual String StripPathSegment(String path);
//        virtual String FileType(String path);
//        virtual String GetFileName(String path, bool withDotType);
//        virtual String SwapFileType(String path, String newType);
//
//        /// Persistent app data (settings, etc.)
//        String ApplicationDataPath(String path, FileManager::PathDomain domain);
//
//        /// Persistent user data, visible to user
//        String UserDataPath(String path, FileManager::PathDomain domain);
//
//        /// Desktop: same folder as app. Mobile: library path.
//        String ApplicationPath(String path);
//
//        /// Temp files path for caches (discardable).
//        String TempFilesPath(String path);
    };
}

#endif
