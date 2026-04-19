#include "MacFileManager.h"

#import <AppKit/AppKit.h>

using namespace PJ;

UP<FileManager> PJ::NewMacFileManager() {
    auto result = NewSDLFileManager();

    result->showFileFunc = [](FilePath path) {
        @autoreleasepool {
            NSString* nsPath = [NSString stringWithUTF8String:path.string().c_str()];
            NSURL* url = [NSURL fileURLWithPath:nsPath];
            [[NSWorkspace sharedWorkspace] activateFileViewerSelectingURLs:@[url]];
        }
    };

    return result;
}
