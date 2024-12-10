#pragma once

#include "SDKIncludes.h"

using namespace PJ;

namespace Kaiju {
    /// Accepts files via drag and drop and runs file processor logic
    class FilesProcessorScene : public Scene {
    public:
        FilesProcessorScene() {}

        void LoadInto(WorldNode& root) override {
            QB(root).Named("Files processor").OrthoStandard().OnDropFiles([](auto args) {
                auto filesProcessor = MAKE<FilesProcessor>(
                    [](auto filePath) { PJ_Dev::IncludeAliasFileProcessor().Process(filePath); },
                    FilesProcessor::Settings(FileSearchType::Recursive)
                );

                auto& event = std::get<1>(args);
                for (auto& filePath : event.filePaths) {
                    filesProcessor->Scan(filePath);
                }

                while (filesProcessor->ProcessNext() == FinishType::Continue) {}
            });
        }
    };
} // namespace Kaiju
