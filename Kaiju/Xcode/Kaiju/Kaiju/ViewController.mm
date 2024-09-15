#import "ViewController.h"
//#import "Kaiju-Swift.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

//    NSViewController *hostingController = [SwiftBridge makeFilesProcessorViewController];
//    [self addChildViewController:hostingController];
//
//    hostingController.view.frame = CGRectMake(0, 0, 500, 500);
//    [self.view addSubview:hostingController.view];
//
//    self.view.translatesAutoresizingMaskIntoConstraints = NO;
//    [[hostingController.view.topAnchor constraintEqualToAnchor:self.view.topAnchor constant:0] setActive:YES];
//    [[hostingController.view.rightAnchor constraintEqualToAnchor:self.view.rightAnchor constant:0] setActive:YES];
//    [[hostingController.view.leftAnchor constraintEqualToAnchor:self.view.leftAnchor constant:0] setActive:YES];
//    [[hostingController.view.bottomAnchor constraintEqualToAnchor:self.view.bottomAnchor constant:0] setActive:YES];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

@end
