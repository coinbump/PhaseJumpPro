#import "AppDelegate.h"
#include "SDKIncludes.h"
#include "SDLKaiju.h"

using namespace PJ;

@interface AppDelegate ()


@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // TODO: rethink how shaders are stored and loaded
    {
        // Must specify full path (resources/pj_shaders, not pj/shaders)
        // Files must have extension
        NSArray *shaderProgramPaths = [[NSBundle mainBundle] pathsForResourcesOfType:@".shprog" inDirectory:@"resources/pj_shaders"];

        // We can't load programs yet, there is no GL context
        for (NSString *path in shaderProgramPaths) {
            GLShaderProgramLoader loader;
            auto programInfo = loader.InfoFromPath([path UTF8String]);
            if (programInfo) {
                Add(GLShaderProgram::Info::registry, programInfo.value());
            }
        }
    }
    
    {
        NSArray *shaderProgramPaths = [[NSBundle mainBundle] pathsForResourcesOfType:@".shprog" inDirectory:@"resources/test/shaders"];

        // We can't load programs yet, there is no GL context
        for (NSString *path in shaderProgramPaths) {
            GLShaderProgramLoader loader;
            auto programInfo = loader.InfoFromPath([path UTF8String]);
            if (programInfo) {
                Add(GLShaderProgram::Info::registry, programInfo.value());
            }
        }
    }

    // A simple test of an SDL window
    KaijuGo();
    
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
