import Foundation
import Testing
@testable import PhaseJumpPro

struct TimerTests {
    class Counter {
        var finishCount = 0
        var resetCount = 0
    }
    
    @Test
    func runOnce() async throws {
        let timer = Timer(1.0, runType: .once)
        timer.onUpdate(time: .init(delta: 0.3))
        #expect(0.3 == timer.state)
        #expect(0.3 == timer.progress)
        #expect(false == timer.isFinished)

        timer.onUpdate(time: .init(delta: 0.7))
        #expect(1.0 == timer.state)
        #expect(1.0 == timer.progress)
        #expect(timer.isFinished)

        timer.onUpdate(time: .init(delta: 0.7))
        #expect(1.0 == timer.state)
        #expect(1.0 == timer.progress)
        #expect(timer.isFinished)

        timer.progress = 0.5
        #expect(false == timer.isFinished)
    }

    @Test
    func testRepeat() async throws {
        let counter = Counter()
        
        let timer = Timer(1.0, runType: .repeat)
        timer.onFinishFunc = { _ in
            counter.finishCount += 1
        }
        timer.onUpdate(time: .init(delta: 0.3))
        #expect(0.3 == timer.state)
        #expect(0.3 == timer.progress)

        timer.onUpdate(time: .init(delta: 0.7))
        #expect(0 == timer.state)
        #expect(0 == timer.progress)
        #expect(1 == counter.finishCount)
        #expect(false == timer.isFinished)

        timer.onUpdate(time: .init(delta: 0.7))
        #expect(0.7 == timer.state)
        #expect(0.7 == timer.progress)
        #expect(false == timer.isFinished)

        timer.onUpdate(time: .init(delta: 0.3))
        #expect(0 == timer.state)
        #expect(0 == timer.progress)
        #expect(2 == counter.finishCount)
        #expect(false == timer.isFinished)
    }

    @Test
    func stop() {
        let timer = Timer(1.0, runType: .once)
        timer.onUpdate(time: .init(delta: 0.3))
        #expect(0.3 == timer.state)
        #expect(0.3 == timer.progress)
        #expect(false == timer.isFinished)

        timer.stop()
        #expect(timer.isFinished)
    }
}
