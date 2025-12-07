#include "gtest/gtest.h"
#include "MediaFilesAtoms.h"

using namespace PJ;

namespace MediaFilesTests {
    struct TestAtomHeader32 {
        uint32_t size{};
        char type[4];
    };
}

using namespace MediaFilesTests;
using namespace PJ;
using namespace MediaFiles;

TEST(MediaFiles, AtomHeaderSize) {
    ASSERT_EQ(sizeof(TestAtomHeader32), sizeof(AtomHeader32));
}
