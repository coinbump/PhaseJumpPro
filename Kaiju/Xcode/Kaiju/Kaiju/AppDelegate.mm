//
//  AppDelegate.m
//  Kaiju
//
//  Created by Jeremy Vineyard on 11/5/22.
//

#import "AppDelegate.h"
#import <PhaseJump/Class.h>
#include "SDLTest.h"

@interface AppDelegate ()


@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // A simple test of an SDL window
    SDLFoo();
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}

@end
