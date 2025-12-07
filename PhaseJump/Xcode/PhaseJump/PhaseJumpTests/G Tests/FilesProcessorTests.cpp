#include "gtest/gtest.h"
#include "FilesProcessor.h"
#include "FileProcessor.Utility.h"

using namespace PJ;
using namespace std;

namespace FilesProcessorTests {
}

using namespace FilesProcessorTests;

TEST(FilesProcessor, Process) {
    FilesProcessor sut({}, FilesProcessor::Config(FileSearchType::Directory));
    sut.Scan(FilePath(""));
    sut.Scan(FilePath(""));
    sut.Scan(FilePath(""));

    EXPECT_EQ(0, sut.Progress());

    sut.ProcessNext();
    EXPECT_EQ(1.0f/3.0f, sut.Progress());

    sut.ProcessNext();
    EXPECT_EQ(2.0f/3.0f, sut.Progress());
}
