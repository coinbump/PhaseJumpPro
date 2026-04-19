#pragma once

#include "Document.h"
#include "VectorList.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/26/24
 */
namespace PJ {
    enum class WrapType { Wrap, None };

    /// Contains a collection of documents
    /// Usually an application has one document bundle for all open documents
    class DocumentBundle {
    protected:
        SP<Document> activeDocument;

        /// The active document
        VectorList<SP<Document>> documents;

    public:
        VectorList<SP<Document>> const& Documents() const {
            return documents;
        }

        size_t Count() const {
            return documents.size();
        }

        SP<Document> ActiveDocument() const {
            return activeDocument;
        }

        /// Adds the document to the bundle. Does not open it
        void Add(SP<Document> document);

        /// Removes the document from the bundle. Does not close it
        void Remove(Document& document);

        /// Removes the document from the bundle and closes it
        void Close(Document& document);

        /// @return Returns a list of modified documents
        VectorList<Document*> ModifiedDocuments() const;

        /// @return Returns true if the bundle contains any modified documents
        bool IsModified() const {
            return !IsEmpty(ModifiedDocuments());
        }

        void SaveModified();

        SP<Document> operator[](size_t index) const {
            GUARDR(index >= 0 && index < documents.size(), {});
            return documents[index];
        }

        void SetActiveDocument(SP<Document> document) {
            GUARD(activeDocument != document)
            activeDocument = document;
        }

        void ActivatePreviousDocument(WrapType wrapType);
        void ActivateNextDocument(WrapType wrapType);
        void SaveActiveDocument();
        void CloseActiveDocument();
    };
} // namespace PJ
