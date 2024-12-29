#include "gtest/gtest.h"
#include "DocumentBundle.h"

using namespace PJ;
using namespace std;

namespace DocumentBundleTests {
}

using namespace DocumentBundleTests;

TEST(DocumentBundle, Add) {
    DocumentBundle sut;
    sut.Add(MAKE<SomeDocument>());
    EXPECT_EQ(1, sut.Count());
}

TEST(DocumentBundle, Remove) {
    DocumentBundle sut;
    
    auto document = MAKE<SomeDocument>();
    
    sut.Add(document);
    EXPECT_EQ(1, sut.Count());
    
    sut.Remove(*document);
    EXPECT_EQ(0, sut.Count());
}

TEST(DocumentBundle, ModifiedDocuments) {
    DocumentBundle sut;
    
    auto document = MAKE<SomeDocument>();
    
    sut.Add(document);
    EXPECT_EQ(0, sut.ModifiedDocuments().size());
    
    document->ModifyPush();
    EXPECT_EQ(1, sut.ModifiedDocuments().size());
}

TEST(DocumentBundle, SaveModified) {
    DocumentBundle sut;
    
    auto document = MAKE<Document<>>();
    
    sut.Add(document);
    document->ModifyPush();
    
    int saveCount{};
    
    document->saveFunc = [&](auto& document) {
        return Void();
    };
    document->onSaveFunc = [&](auto& document) {
        saveCount++;
    };
    
    sut.SaveModified();
    EXPECT_EQ(1, saveCount);
}
