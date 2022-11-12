//
//  PhaseJumpTests.m
//  PhaseJumpTests
//
//  Created by Jeremy Vineyard on 11/5/22.
//

#import <XCTest/XCTest.h>
#include "Types/_String.h"
#include "Types/FloatClamp.h"
#include "Types/FloatTransform.h"
#include "Animation/AnimationCycleType.h"
#include "Core/Updatable.h"
#include "Model/SomeTimed.h"
#include "Model/Timer.h"
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
