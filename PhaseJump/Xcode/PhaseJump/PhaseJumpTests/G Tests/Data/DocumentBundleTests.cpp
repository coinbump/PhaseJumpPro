#include "gtest/gtest.h"
#include "DocumentBundle.h"

using namespace PJ;
using namespace std;

namespace DocumentBundleTests {
}

using namespace DocumentBundleTests;

TEST(DocumentBundle, Add) {
    DocumentBundle sut;
    sut.Add(MAKE<Document>());
    EXPECT_EQ(1, sut.Count());
}

TEST(DocumentBundle, Remove) {
    DocumentBundle sut;
    
    auto document = MAKE<Document>();
    
    sut.Add(document);
    EXPECT_EQ(1, sut.Count());
    
    sut.Remove(*document);
    EXPECT_EQ(0, sut.Count());
}

TEST(DocumentBundle, ModifiedDocuments) {
    DocumentBundle sut;
    
    auto document = MAKE<Document>();
    
    sut.Add(document);
    EXPECT_EQ(0, sut.ModifiedDocuments().size());
    
    document->ModifyPush();
    EXPECT_EQ(1, sut.ModifiedDocuments().size());
}

TEST(DocumentBundle, SaveModified) {
    DocumentBundle sut;
    
    auto document = MAKE<CoreDocument<>>();
    
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

TEST(DocumentBundle, Close) {
    DocumentBundle sut;

    auto doc1 = MAKE<Document>();
    auto doc2 = MAKE<Document>();
    auto doc3 = MAKE<Document>();

    sut.Add(doc1);
    sut.Add(doc2);
    sut.Add(doc3);
    sut.SetActiveDocument(doc2);

    sut.Close(*doc2);
    EXPECT_EQ(2, sut.Count());
    EXPECT_EQ(doc1, sut.ActiveDocument());
}

TEST(DocumentBundle, Close_NoActiveDocument) {
    DocumentBundle sut;

    auto doc1 = MAKE<Document>();
    sut.Add(doc1);

    sut.Close(*doc1);
    EXPECT_EQ(0, sut.Count());
    EXPECT_EQ(nullptr, sut.ActiveDocument());
}

TEST(DocumentBundle, Close_LastDocument) {
    DocumentBundle sut;

    auto doc1 = MAKE<Document>();
    sut.Add(doc1);
    sut.SetActiveDocument(doc1);

    sut.Close(*doc1);
    EXPECT_EQ(0, sut.Count());
    EXPECT_EQ(nullptr, sut.ActiveDocument());
}

TEST(DocumentBundle, SetActiveDocument) {
    DocumentBundle sut;

    auto doc1 = MAKE<Document>();
    auto doc2 = MAKE<Document>();

    sut.Add(doc1);
    sut.Add(doc2);

    EXPECT_EQ(nullptr, sut.ActiveDocument());

    sut.SetActiveDocument(doc1);
    EXPECT_EQ(doc1, sut.ActiveDocument());

    sut.SetActiveDocument(doc2);
    EXPECT_EQ(doc2, sut.ActiveDocument());
}

TEST(DocumentBundle, ActivatePreviousDocument) {
    DocumentBundle sut;

    auto doc1 = MAKE<Document>();
    auto doc2 = MAKE<Document>();
    auto doc3 = MAKE<Document>();

    sut.Add(doc1);
    sut.Add(doc2);
    sut.Add(doc3);
    sut.SetActiveDocument(doc2);

    sut.ActivatePreviousDocument(WrapType::None);
    EXPECT_EQ(doc1, sut.ActiveDocument());
}

TEST(DocumentBundle, ActivatePreviousDocument_AtFirstDocument_NoWrap) {
    DocumentBundle sut;

    auto doc1 = MAKE<Document>();
    auto doc2 = MAKE<Document>();

    sut.Add(doc1);
    sut.Add(doc2);
    sut.SetActiveDocument(doc1);

    sut.ActivatePreviousDocument(WrapType::None);
    
    EXPECT_EQ(doc1, sut.ActiveDocument());
}

TEST(DocumentBundle, ActivatePreviousDocument_AtFirstDocument_Wrap) {
    DocumentBundle sut;

    auto doc1 = MAKE<Document>();
    auto doc2 = MAKE<Document>();

    sut.Add(doc1);
    sut.Add(doc2);
    sut.SetActiveDocument(doc1);

    sut.ActivatePreviousDocument(WrapType::Wrap);
    
    EXPECT_EQ(doc2, sut.ActiveDocument());
}

TEST(DocumentBundle, ActivatePreviousDocument_Empty) {
    DocumentBundle sut;

    sut.ActivatePreviousDocument(WrapType::Wrap);
    EXPECT_EQ(nullptr, sut.ActiveDocument());
}

TEST(DocumentBundle, ActivateNextDocument) {
    DocumentBundle sut;

    auto doc1 = MAKE<Document>();
    auto doc2 = MAKE<Document>();

    sut.Add(doc1);
    sut.Add(doc2);
    sut.SetActiveDocument(doc1);

    sut.ActivateNextDocument(WrapType::Wrap);

    EXPECT_EQ(doc2, sut.ActiveDocument());
}

TEST(DocumentBundle, ActivateNextDocument_NoWrap) {
    DocumentBundle sut;

    auto doc1 = MAKE<Document>();
    auto doc2 = MAKE<Document>();

    sut.Add(doc1);
    sut.Add(doc2);
    sut.SetActiveDocument(doc2);

    sut.ActivateNextDocument(WrapType::None);
    EXPECT_EQ(doc2, sut.ActiveDocument());
}

TEST(DocumentBundle, ActivateNextDocument_Wrap) {
    DocumentBundle sut;

    auto doc1 = MAKE<Document>();
    auto doc2 = MAKE<Document>();

    sut.Add(doc1);
    sut.Add(doc2);
    sut.SetActiveDocument(doc2);

    sut.ActivateNextDocument(WrapType::Wrap);
    EXPECT_EQ(doc1, sut.ActiveDocument());
}

TEST(DocumentBundle, ActivateNextDocument_Empty) {
    DocumentBundle sut;

    sut.ActivateNextDocument(WrapType::Wrap);
    EXPECT_EQ(nullptr, sut.ActiveDocument());
}

TEST(DocumentBundle, SaveActiveDocument) {
    DocumentBundle sut;

    auto document = MAKE<CoreDocument<>>();
    sut.Add(document);
    sut.SetActiveDocument(document);
    document->ModifyPush();

    int saveCount{};

    document->saveFunc = [&](auto& document) {
        return Void();
    };
    document->onSaveFunc = [&](auto& document) {
        saveCount++;
    };

    sut.SaveActiveDocument();
    EXPECT_EQ(1, saveCount);
}

TEST(DocumentBundle, SaveActiveDocument_NoActiveDocument) {
    DocumentBundle sut;

    // Should be a no-op with no active document
    sut.SaveActiveDocument();
    EXPECT_EQ(nullptr, sut.ActiveDocument());
}

TEST(DocumentBundle, CloseActiveDocument) {
    DocumentBundle sut;

    auto doc1 = MAKE<Document>();
    auto doc2 = MAKE<Document>();

    sut.Add(doc1);
    sut.Add(doc2);
    sut.SetActiveDocument(doc2);

    sut.CloseActiveDocument();
    EXPECT_EQ(1, sut.Count());
    EXPECT_EQ(doc1, sut.ActiveDocument());
}

TEST(DocumentBundle, CloseActiveDocument_NoActiveDocument) {
    DocumentBundle sut;

    auto doc1 = MAKE<Document>();
    sut.Add(doc1);

    sut.CloseActiveDocument();
    EXPECT_EQ(1, sut.Count());
}

TEST(DocumentBundle, CloseActiveDocument_LastDocument) {
    DocumentBundle sut;

    auto doc1 = MAKE<Document>();
    sut.Add(doc1);
    sut.SetActiveDocument(doc1);

    sut.CloseActiveDocument();
    EXPECT_EQ(0, sut.Count());
    EXPECT_EQ(nullptr, sut.ActiveDocument());
}
