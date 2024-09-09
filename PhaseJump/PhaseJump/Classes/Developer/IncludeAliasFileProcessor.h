#ifndef PJINCLUDEALIASFILEPROCESSOR_H
#define PJINCLUDEALIASFILEPROCESSOR_H

#include "FilesProcessor.h"

using namespace PJ;

// CODE REVIEW: ?/23
namespace PJ_Dev {
    /**
     Scans for .h files and creates an "alias" .h file with a link to the .h
     file in the project directory Needed because PhaseJump .h files are
     hierarchical, but IDEs like Visual Studio don't support recursive includes
     So we need a flat includes directory
     */
    class IncludeAliasFileProcessor : public SomeFileProcessor {
    public:
        void Process(FilePath filePath) override;
    };
} // namespace PJ_Dev

#endif
