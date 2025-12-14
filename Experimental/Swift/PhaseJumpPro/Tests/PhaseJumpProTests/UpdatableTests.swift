import Foundation
import Testing
@testable import PhaseJumpPro

struct UpdatableTests {
    class Counter {
        var finishCount = 0
        var didFinish = false
        var timeValue = 0.0
    }
    
    @Test
    func isFinished() async throws {
        let counter = Counter()
        let sut = Updatable()
        
        sut.onFinishFunc = {
            counter.didFinish = true
        }
        sut.onUpdateFunc = { _ in
            .finish
        }
        
        #expect(false == sut.isFinished)
        sut.onUpdate(time: .init(delta: 1))
        #expect(sut.isFinished)
        #expect(counter.didFinish)
    }
    
    @Test
    func finish() async throws {
        let counter = Counter()
        let sut = Updatable()
        
        sut.onFinishFunc = {
            counter.didFinish = true
        }
        #expect(false == sut.isFinished)
        sut.finish()
        #expect(sut.isFinished)
        #expect(counter.didFinish)
    }
    
    @Test
    func update() async throws {
        let counter = Counter()
        let sut = Updatable()
        
        sut.onUpdateFunc = { time in
            counter.timeValue += time.delta
            return .continue
        }
        #expect(false == sut.isFinished)
        
        sut.onUpdate(time: .init(delta: 3))
        #expect(3 == counter.timeValue)
    }
    
    @Test
    func updateAfterfinish() async throws {
        let counter = Counter()
        let sut = Updatable()
        
        sut.onUpdateFunc = { time in
            counter.timeValue += time.delta
            return .continue
        }
        sut.finish()
        #expect(sut.isFinished)
        
        sut.onUpdate(time: .init(delta: 3))
        #expect(0 == counter.timeValue)
    }
}
