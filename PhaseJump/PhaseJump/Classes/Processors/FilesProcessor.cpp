#include "FilesProcessor.h"

using namespace std;
using namespace PJ;

FilesProcessor::FilesProcessor(ProcessFileFunc processFileFunc, Config const& config) :
    processFileFunc(processFileFunc),
    config(config) {}

void FilesProcessor::Scan(FilePath path) {
    if (fs::is_directory(path)) {
        switch (config.fileSearchType) {
        case FileSearchType::Recursive:
            for (auto& path : fs::recursive_directory_iterator{ path }) {
                AddFile(path);
            }
            break;
        case FileSearchType::Directory:
            for (auto& path : fs::directory_iterator{ path }) {
                AddFile(path);
            }
            break;
        case FileSearchType::File:
            AddFile(path);
            break;
        }
    } else {
        AddFile(path);
    }
}

void FilesProcessor::AddFile(FilePath path) {
    Add(input, path);
}

void FilesProcessor::Process(FilePath path) {
    GUARD(processFileFunc)
    processFileFunc(path);
}

FinishType FilesProcessor::ProcessNext() {
    if (processedCount >= 0 && processedCount < input.size()) {
        Process(input[processedCount]);
        processedCount++;
    }

    return processedCount < input.size() ? FinishType::Continue : FinishType::Finish;
}
