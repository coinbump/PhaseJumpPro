#include "gtest/gtest.h"
#include "FilesProcessor.h"
#include "FileProcessor.Utility.h"

using namespace PJ;
using namespace std;

namespace FilesProcessorTests {
    class TestFileProcessor: public SomeFileProcessor {
    public:
        void Process(FilePath filePath) override {
        }
    };
}

using namespace FilesProcessorTests;

TEST(FilesProcessor, Process) {
    UP<SomeFileProcessor> fileProcessor = std::make_unique<TestFileProcessor>();
    FilesProcessor sut(3, fileProcessor, FilesProcessor::Settings(FileSearchType::Directory));
    sut.Provide(FilePath(""));
    sut.Provide(FilePath(""));
    sut.Provide(FilePath(""));

    EXPECT_EQ(0, sut.Progress());

    sut.ProcessNext();
    EXPECT_EQ(1.0f/3.0f, sut.Progress());

    sut.ProcessNext();
    EXPECT_EQ(2.0f/3.0f, sut.Progress());
}
