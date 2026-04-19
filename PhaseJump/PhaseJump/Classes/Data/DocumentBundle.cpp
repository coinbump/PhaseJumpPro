#include "DocumentBundle.h"
#include "CollectionUtils.h"

using namespace std;
using namespace PJ;

VectorList<Document*> DocumentBundle::ModifiedDocuments() const {
    VectorList<Document*> result =
        Map<Document*>(documents, [](auto& document) { return document.get(); });
    result = Filter(result, [](auto& document) { return document->IsModified(); });
    return result;
}

void DocumentBundle::SaveModified() {
    auto documents = ModifiedDocuments();
    for (auto& document : documents) {
        document->Save();
    }
}

void DocumentBundle::Add(SP<Document> document) {
    GUARD(document)

    documents.push_back(document);
}

void DocumentBundle::Remove(Document& _document) {
    documents = Filter(documents, [&](auto& document) { return &_document != document.get(); });
}

void DocumentBundle::Close(Document& document) {
    Remove(document);

    if (activeDocument) {
        if (!IsEmpty(documents)) {
            SetActiveDocument(documents[0]);
        } else {
            SetActiveDocument({});
        }
    }
}

void DocumentBundle::ActivatePreviousDocument(WrapType wrapType) {
    GUARD(documents.size() > 0)

    if (activeDocument) {
        auto index = IndexOf(documents, activeDocument);
        if (index) {
            if (index == 0) {
                if (wrapType == WrapType::Wrap) {
                    SetActiveDocument(SCAST<Document>(documents[documents.size() - 1]));
                }
            } else {
                SetActiveDocument(SCAST<Document>(documents[(int)*index - 1]));
            }
        }
        return;
    }
}

void DocumentBundle::ActivateNextDocument(WrapType wrapType) {
    GUARD(documents.size() > 0)

    if (activeDocument) {
        auto index = IndexOf(documents, activeDocument);
        if (index) {
            auto nextIndex = *index + 1;
            if (nextIndex > documents.size() - 1) {
                if (wrapType == WrapType::Wrap) {
                    SetActiveDocument(SCAST<Document>(documents[0]));
                }
            } else {
                SetActiveDocument(SCAST<Document>(documents[nextIndex]));
            }
        }
        return;
    }
}

void DocumentBundle::SaveActiveDocument() {
    GUARD(activeDocument)
    activeDocument->Save();
}

void DocumentBundle::CloseActiveDocument() {
    GUARD(activeDocument)
    Close(*activeDocument);
}
