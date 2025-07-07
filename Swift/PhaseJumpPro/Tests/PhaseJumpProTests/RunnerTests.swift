import Foundation
import Testing
@testable import PhaseJumpPro
import XCTest

final class MyLibraryTests: XCTestCase {
    func testExample() {
        XCTAssertEqual(2 + 2, 4)
    }
}

// TODO: Can't get these tests to run, no way to verify (yet)

struct RunnerTests {
    class Counter {
        var finishCount = 0
        var resetCount = 0
    }
    
    @Test
    func runOnce() async throws {
        var sut = Runner(runType: .once)
        let counter = Counter()
        
        sut.onFinishFunc = { counter.finishCount += 1 }
        sut.onResetFunc = { counter.resetCount += 1 }

        #expect(false == sut.isFinished)
        sut.reset()
        #expect(false == sut.isFinished)
        #expect(0 == counter.finishCount)
        sut.finish()
        #expect(1 == counter.finishCount)
        #expect(0 == counter.resetCount)

        sut.finish()
        #expect(1 == counter.finishCount)
        #expect(0 == counter.resetCount)
    }

    @Test
    func testRepeat() async throws {
        var sut = Runner(runType: .repeat)
        let counter = Counter()
        
        sut.onFinishFunc = { counter.finishCount += 1 }
        sut.onResetFunc = { counter.resetCount += 1 }

        #expect(false == sut.isFinished)
        sut.reset()
        #expect(false == sut.isFinished)
        #expect(0 == counter.finishCount)
        sut.finish()
        #expect(1 == counter.finishCount)
        #expect(1 == counter.resetCount)
        #expect(false == sut.isFinished)

        sut.finish()
        #expect(2 == counter.finishCount)
        #expect(2 == counter.resetCount)
        #expect(false == sut.isFinished)
    }
}
