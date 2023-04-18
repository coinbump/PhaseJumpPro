//
//  AppDelegate.m
//  Kaiju
//
//  Created by Jeremy Vineyard on 11/5/22.
//

#import "AppDelegate.h"
#import <PhaseJump/PhaseJump.h>
#include "SDLTest.h"

using namespace PJ;

@interface AppDelegate ()


@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Must specify full path (resources/pj_shaders, not pj/shaders)
    // Files must have extension
    NSArray *shaderProgramPaths = [[NSBundle mainBundle] pathsForResourcesOfType:@".shprog" inDirectory:@"resources/pj_shaders"];

    // We can't load programs yet, there is no GL context
    for (NSString *path in shaderProgramPaths) {
        GLShaderProgramLoader loader;
        auto programInfo = loader.InfoFromPath([path UTF8String]);
        if (programInfo) {
            GLShaderProgram::Info::registry.Add(programInfo.value());
        }
    }

    // A simple test of an SDL window
    SDLFoo();
    
    SQLDatabase db;
//
//    NSString* libraryPath;
//    libraryPath = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES)[0];
//
//    FilePath folderPath([libraryPath UTF8String]);
//    folderPath.append("Test");
//
//    FileManager fm;
//    fm.CreateDirectories(folderPath);
//
//    folderPath.append("Hello.sqlite");


//    GLShaderProgramsLoader loader;
//    loader.LoadAllProgramsFromPath(
    
//    try {
//        db.TryOpen(folderPath, SQLDatabaseOpenType::AtPath, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
//    }
//    catch (int error) {
//        std::cout << "Error" << error;
//    }
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}

@end
