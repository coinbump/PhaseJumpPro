#include "UmbrellaHeadersFilesProcessor.h"
#include "File.h"
#include "SDLFileManager.h"
#include "StringUtils.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace PJ;
using namespace PJ_Dev;

UmbrellaHeadersFilesProcessor::UmbrellaHeadersFilesProcessor() :
    Base({}, { .fileSearchType = FileSearchType::File }) {}

void UmbrellaHeadersFilesProcessor::Process(FilePath filePath) {
    FilePath classesFilePath = filePath;
    classesFilePath /= "PhaseJump/Classes";

    SDLFileManager fm;
    auto pathList = fm.FilePathList(classesFilePath, FileSearchType::Recursive);

    auto process = [&](String prefix) {
        stringstream ss;
        VectorList<String> exceptions{ "RTexPackerAtlasModelBuilder" };

        VectorList<String> fileNames;
        for (auto& path : pathList) {
            auto pathString = path.string();
            GUARD_CONTINUE(!pathString.contains("_Experimental"));

            bool isContinue = false;
            for (auto& exception : exceptions) {
                if (pathString.contains(exception)) {
                    isContinue = true;
                    break;
                }
            }
            GUARD_CONTINUE(!isContinue)

            auto extension = fm.FileExtension(path, FileManager::FileExtensionFormat::NoDot);
            if ("h" == extension) {
                auto fileName = fm.FileName(path, FileManager::FileNameFormat::WithExtension);
                fileNames.push_back(fileName);
            }
        }

        std::sort(fileNames.begin(), fileNames.end(), [](auto& lhs, auto& rhs) {
            return lhs < rhs;
        });

        for (auto fileName : fileNames) {
            ss << "#include <" << prefix << "/" << fileName << ">" << std::endl;
        }

        FilePath headerPath = filePath;
        headerPath /= "Xcode/PhaseJump/PhaseJump";
        String umbrellaHeaderName = prefix + ".h";
        headerPath /= umbrellaHeaderName;

        std::ofstream file(headerPath);
        file << ss.str();
    };

    process("PhaseJump");
    process("PhaseJump-Dev");
}
