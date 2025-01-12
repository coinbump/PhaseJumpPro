#import "AppDelegate.h"
#include "SDKIncludes.h"
#include "SDLKaiju.h"

using namespace PJ;

@interface AppDelegate ()


@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    KaijuGo();
    
    // Example: App::New().qb.Circle(100, Color::red);
    
//    SQLDatabase db;
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
