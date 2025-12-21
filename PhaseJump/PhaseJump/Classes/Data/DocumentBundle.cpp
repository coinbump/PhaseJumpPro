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
