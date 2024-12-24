#include "gtest/gtest.h"
#include "Document.h"

using namespace PJ;
using namespace std;

namespace DocumentTests {
}

using namespace DocumentTests;

TEST(Document, OnCloseFunc) {
    int callCount{};
    Document sut;
    
    sut.onCloseFunc = [&](auto& document) {
        callCount++;
    };
    
    sut.Close();
    
    EXPECT_EQ(1, callCount);
}

TEST(Document, Modify) {
    Document sut;
    
    EXPECT_FALSE(sut.IsModified());
    sut.ModifyPush();
    EXPECT_TRUE(sut.IsModified());
    sut.ModifyPop();
    EXPECT_FALSE(sut.IsModified());
}

TEST(Document, Load) {
    int callCount{};
    int callCount2{};
    Document sut;
    
    sut.loadFunc = [&](auto& document) {
        callCount++;
        return Void();
    };
    sut.onLoadFunc = [&](auto& document) {
        callCount2++;
    };
    
    EXPECT_FALSE(sut.IsLoaded());
    sut.Load();
    
    EXPECT_EQ(1, callCount);
    EXPECT_EQ(1, callCount2);
    EXPECT_TRUE(sut.IsLoaded());
}

TEST(Document, LoadFail) {
    int callCount{};
    int callCount2{};
    Document sut;
    
    sut.loadFunc = [&](auto& document) {
        callCount++;
        return Document::Failure();
    };
    sut.onLoadFunc = [&](auto& document) {
        callCount2++;
    };
    
    EXPECT_FALSE(sut.IsLoaded());
    sut.Load();
    
    EXPECT_EQ(1, callCount);
    EXPECT_EQ(0, callCount2);
    EXPECT_FALSE(sut.IsLoaded());
}

TEST(Document, Save) {
    int callCount{};
    int callCount2{};
    Document sut;
    
    sut.saveFunc = [&](auto& document) {
        callCount++;
        return Void();
    };
    sut.onSaveFunc = [&](auto& document) {
        callCount2++;
    };
    
    sut.Save();
    
    EXPECT_EQ(1, callCount);
    EXPECT_EQ(1, callCount2);
}

TEST(Document, SaveFail) {
    int callCount{};
    int callCount2{};
    Document sut;
    
    sut.saveFunc = [&](auto& document) {
        callCount++;
        return Document::Failure();
    };
    sut.onSaveFunc = [&](auto& document) {
        callCount2++;
    };
    
    sut.Save();
    
    EXPECT_EQ(1, callCount);
    EXPECT_EQ(0, callCount2);
}

