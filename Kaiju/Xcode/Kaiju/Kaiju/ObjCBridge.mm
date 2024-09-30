#import "ObjCBridge.h"

#if DEVELOPMENT
#include <PhaseJump-Dev/PhaseJump-Dev.h>
#else
#include <PhaseJump/PhaseJump.h>
#endif

using namespace PJ;
using namespace std;

ObjCBridge* _sharedInstance;
SP<FilesProcessor> filesProcessor;

@implementation ObjCBridge

// Future: support generic operations with parameters: Swift -> ObjC -> C++ bridge
- (void)startFilesProcessorWithFilePathsCount:(NSInteger)filePathsCount
{
//    UP<FileProcessor> fileProcessor = std::make_unique<PJ_Dev::IncludeAliasFileProcessor>();
//    filesProcessor = MAKE<FilesProcessor>(filePathsCount, fileProcessor, FilesProcessor::Settings(FileSearchType::Recursive));
}

- (void)provideFilePath:(NSString*)filePath
{
    if (!filesProcessor) { return; }
    filesProcessor->Scan(FilePath([filePath UTF8String]));
}

- (void)processNextFile {
    if (!filesProcessor) { return; }
    while (filesProcessor->ProcessNext() == FinishType::Continue) {}
}

+ (ObjCBridge*)shared
{
    @synchronized([ObjCBridge class]) {
        if (!_sharedInstance)
            _sharedInstance = [self new];
        return _sharedInstance;
    }
    return nil;
}

@end

@interface PJOperation

@property (nonatomic, strong) NSString* _id;
@property (nonatomic, strong) NSDictionary* parameters;

@end
