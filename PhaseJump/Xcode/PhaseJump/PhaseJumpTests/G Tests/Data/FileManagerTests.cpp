#include "gtest/gtest.h"
#include "FileManager.h"
#include "SDLFileManager.h"
#include "Utils.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace FileManagerTests {
}

using namespace FileManagerTests;

TEST(FileManager, FileExtension) {
    SDLFileManager sut;
    EXPECT_EQ("png", sut.FileExtension("test.png", false));
    EXPECT_EQ(".png", sut.FileExtension("test.png", true));
    EXPECT_EQ("png", sut.FileExtension("test.pdf.png", false));
    EXPECT_EQ(".png", sut.FileExtension("test.pdf.png", true));
}

TEST(FileManager, FileName) {
    SDLFileManager sut;
    EXPECT_EQ("test", sut.FileName("file://path/test.png", false));
    EXPECT_EQ("test.png", sut.FileName("file://path/test.png", true));
}
