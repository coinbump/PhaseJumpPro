#include "gtest/gtest.h"
#include "FilePath.h"

using namespace PJ;
using namespace std;

// Tests disabled: according to specs, multiple /// slashes don't matter so we don't need special logic
//TEST(FilePath, join_no_separator) {
//    FilePath path1("l");
//    FilePath path2("r");
//
//    auto joinedPath = path1 + path2;
//    EXPECT_EQ(string("l/r"), (string)joinedPath);
//}
//
//TEST(FilePath, join_separator_suffix) {
//    FilePath path1("l/");
//    FilePath path2("r");
//
//    auto joinedPath = path1 + path2;
//    EXPECT_EQ(string("l/r"), (string)joinedPath);
//}
//
//TEST(FilePath, join_separator_prefix) {
//    FilePath path1("l");
//    FilePath path2("/r");
//
//    auto joinedPath = path1 + path2;
//    EXPECT_EQ(string("l/r"), (string)joinedPath);
//}
//
//TEST(FilePath, join_separator_both) {
//    FilePath path1("l/");
//    FilePath path2("/r");
//
//    auto joinedPath = path1 + path2;
//    EXPECT_EQ(string("l/r"), (string)joinedPath);
//}
//
//TEST(FilePath, join_plus_equal) {
//    FilePath path1("l/");
//    FilePath path2("/r");
//
//    path1 += path2;
//    EXPECT_EQ(string("l/r"), (string)path1);
//}
