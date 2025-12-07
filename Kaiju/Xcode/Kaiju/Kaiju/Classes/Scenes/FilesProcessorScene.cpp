#include "FilesProcessorScene.h"

using namespace std;
using namespace PJ;
using namespace Kaiju;

void FilesProcessorScene::LoadInto(WorldNode& root) {
    QB(root).Named("Files processor").OrthoStandard().OnDropFiles([](auto args) {
        //                auto filesProcessor = MAKE<FilesProcessor>(
        //                    [](auto filePath) {
        //                    PJ_Dev::IncludeAliasFileProcessor().Process(filePath); },
        //                    FilesProcessor::Settings(FileSearchType::Recursive)
        //                );
        //
        auto filesProcessor = MAKE<PJ_Dev::UmbrellaHeadersFilesProcessor>();

        auto& event = args.event;
        for (auto& filePath : event.filePaths) {
            filesProcessor->Scan(filePath);
        }

        while (filesProcessor->ProcessNext() == FinishType::Continue) {}
    });
}
