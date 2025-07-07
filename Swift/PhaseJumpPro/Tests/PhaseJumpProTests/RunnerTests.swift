import Foundation
import Testing
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
        let sut = Runner(runType: .once)
        let counter = Counter()
        
        sut.onFinishFunc = { counter.finishCount++ }
        sut.onResetFunc = { counter.resetCount++ }

        #expect(false == sut.isFinished)
        sut.isFinished = false
        #expect(false == sut.isFinished)
        #expect(0 == counter.finishCount)
        sut.isFinished = true
        #expect(1 == counter.finishCount)
        #expect(0 == counter.resetCount)

        sut.isFinished = true
        #expect(1 == counter.finishCount)
        #expect(0 == counter.resetCount)
    }

    @Test
    func repeat() async throws {
        let sut = Runner(runType: .repeat)
        let counter = Counter()
        
        sut.onFinishFunc = { counter.finishCount++ }
        sut.onResetFunc = { counter.resetCount++ }

        #expect(false == sut.isFinished)
        sut.isFinished = false
        #expect(false == sut.isFinished)
        #expect(0 == counter.finishCount)
        sut.isFinished = true
        #expect(1 == counter.finishCount)
        #expect(1 == counter.resetCount)
        #expect(false == sut.isFinished)

        sut.isFinished = true
        #expect(2 == counter.finishCount)
        #expect(2 == counter.resetCount)
        #expect(false == sut.isFinished)
    }
}
