#import <XCTest/XCTest.h>
#include "StringUtils.h"
#include "gtest/gtest.h"

using namespace PJ;

@interface FloatTransformTests : XCTestCase

@end

@implementation FloatTransformTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)test_googleTest {
    // Insert code here to initialize your application
    ::testing::InitGoogleTest();
    XCTAssertEqual(0, RUN_ALL_TESTS());
}

@end
