//
//  OjObjCBridgebCBridge.h
//  Kaiju
//
//  Created by Jeremy Vineyard on 11/12/22.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ObjCBridge : NSObject

- (void)startFilesProcessorWithFilePathsCount:(NSInteger)filePathsCount;
- (void)provideFilePath:(NSString*)filePath;
- (void)processNextFile;

+ (ObjCBridge*)shared;

@end

NS_ASSUME_NONNULL_END
