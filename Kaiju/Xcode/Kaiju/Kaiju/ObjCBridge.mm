//
//  ObjCBridge.m
//  Kaiju
//
//  Created by Jeremy Vineyard on 11/12/22.
//

#import "ObjCBridge.h"
#import <PhaseJump/FileProcessor.Utility.h>
#import <PhaseJump/FilePath.h>

using namespace PJ;
using namespace std;

ObjCBridge* _sharedInstance;
shared_ptr<FilesProcessor> filesProcessor;

@implementation ObjCBridge

// Future: support generic operations with parameters: Swift -> ObjC -> C++ bridge
- (void)startFilesProcessorWithFilePathsCount:(NSInteger)filePathsCount
{
    filesProcessor = make_shared<FilesProcessor>(filePathsCount, make_shared<FileProcessor::Utility::RenameCSToHFileProcessor>());
}

- (void)provideFilePath:(NSString*)filePath
{
    if (!filesProcessor) { return; }
    filesProcessor->Provide(FilePath([filePath UTF8String]));
}

- (void)processNextFile {
    if (!filesProcessor) { return; }
    while (filesProcessor->ProcessNext()) {}
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
