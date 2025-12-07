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
    EXPECT_EQ("png", sut.FileExtension("test.png", FileManager::FileExtensionFormat::NoDot));
    EXPECT_EQ(".png", sut.FileExtension("test.png", FileManager::FileExtensionFormat::WithDot));
    EXPECT_EQ("png", sut.FileExtension("test.pdf.png", FileManager::FileExtensionFormat::NoDot));
    EXPECT_EQ(".png", sut.FileExtension("test.pdf.png", FileManager::FileExtensionFormat::WithDot));
}

TEST(FileManager, FileName) {
    SDLFileManager sut;
    EXPECT_EQ("test", sut.FileName("file://path/test.png", FileManager::FileNameFormat::NoExtension));
    EXPECT_EQ("test.png", sut.FileName("file://path/test.png", FileManager::FileNameFormat::WithExtension));
}
