//
//  AppDelegate.m
//  Kaiju
//
//  Created by Jeremy Vineyard on 11/5/22.
//

#import "AppDelegate.h"
#import <PhaseJump/Class.h>
#include "SDLTest.h"
#include <PhaseJump/SQLDatabase.h>
#include <PhaseJump/FileManager.h>
#include <PhaseJump/FilePath.h>

using namespace PJ;

@interface AppDelegate ()


@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // A simple test of an SDL window
    SDLFoo();
    
    SQLDatabase db;

    NSString* libraryPath;
    libraryPath = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES)[0];

    FilePath folderPath([libraryPath UTF8String]);
    folderPath.append("Test");

    FileManager fm;
    fm.CreateDirectories(folderPath);

    folderPath.append("Hello.sqlite");

    try {
        db.TryOpen(folderPath, SQLDatabaseOpenType::AtPath, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    }
    catch (int error) {
        std::cout << "Error" << error;
    }
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}

@end
