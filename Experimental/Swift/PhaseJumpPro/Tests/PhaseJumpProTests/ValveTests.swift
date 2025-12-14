import Foundation
import Testing
@testable import PhaseJumpPro

struct ValveTests {
    class Counter {
        var progress = 0.0
    }
    
    func makeTestValve(counter: Counter, isOn: Bool) -> Valve {
        var result = Valve(isOn: isOn)
        result.onValveUpdateFunc = {
            counter.progress = $0.value
        }
        
        return result
    }

    @Test
    func initializeIsOn() {
        let counter = Counter()
        let sut = makeTestValve(counter: counter, isOn: true)
        #expect(sut.isOn)
        #expect(1 == counter.progress)
    }

    @Test
    func initializeIsOff() {
        let counter = Counter()
        let sut = makeTestValve(counter: counter, isOn: false)
        #expect(false == sut.isOn)
        #expect(0 == counter.progress)
    }
    
    @Test
    func turnOnWhenOff() {
        let counter = Counter()
        let sut = makeTestValve(counter: counter, isOn: false)
        #expect(false == sut.isOn)

        sut.turnOn(duration: 2)
        #expect(.turnOn == sut.state)
        #expect(0 == sut.value)

        sut.onUpdate(time: .init(delta: 1))
        #expect(0.5 == sut.value)
        #expect(0.5 == counter.progress)

        sut.onUpdate(time: .init(delta: 1))
        #expect(1.0 == sut.value)
        #expect(1.0 == counter.progress)

        #expect(sut.isOn)
    }

    @Test
    func turnOnInterruptTurnOff() {
        let sut = Valve(isOn: false)
        #expect(false == sut.isOn)

        sut.turnOn(duration: 2)
        #expect(.turnOn == sut.state)
        #expect(0 == sut.value)

        sut.onUpdate(time: .init(delta: 0.5))
        #expect(0.25 == sut.value)

        sut.turnOff(duration: 2)
        #expect(.turnOff == sut.state)
        #expect(0.25 == sut.value)

        sut.onUpdate(time: .init(delta: 0.5))
        #expect(0 == sut.value)
        #expect(sut.isOff)
    }

    @Test
    func turnOnWhenOn() {
        let sut = Valve(isOn: true)
        #expect(sut.isOn)

        sut.turnOn(duration: 2)
        #expect(1.0 == sut.value)
        #expect(sut.isOn)
    }

    @Test
    func turnOffWhenOn() {
        let sut = Valve(isOn: true)
        #expect(false == sut.isOff)

        sut.turnOff(duration: 2)
        #expect(.turnOff == sut.state)
        #expect(1 == sut.value)

        sut.onUpdate(time: .init(delta: 0.5))
        #expect(0.75 == sut.value)

        sut.onUpdate(time: .init(delta: 1.5))
        #expect(0 == sut.value)

        #expect(sut.isOff)
    }

    @Test
    func turnOffWhenOff() {
        let sut = Valve(isOn: false)
        #expect(sut.isOff)

        sut.turnOff(duration: 2)
        #expect(0 == sut.value)
        #expect(sut.isOff)
    }
    
    @Test
    func turnOffInterruptTurnOn() {
        let sut = Valve(isOn: true)
        #expect(sut.isOn)

        sut.turnOff(duration: 2)
        #expect(.turnOff == sut.state)
        #expect(1 == sut.value)

        sut.onUpdate(time: .init(delta: 0.5))
        #expect(0.75 == sut.value)

        sut.turnOn(duration: 2)
        #expect(.turnOn == sut.state)
        #expect(0.75 == sut.value)

        sut.onUpdate(time: .init(delta: 0.5))
        #expect(1 == sut.value)
        #expect(sut.isOn)
    }

    @Test
    func turnOnImmediate() {
        let counter = Counter()
        let sut = makeTestValve(counter: counter, isOn: false)
        #expect(false == sut.isOn)

        sut.turnOn(duration: 0)
        #expect(.on == sut.state)
        #expect(1 == sut.value)
        #expect(1 == counter.progress)
    }
    
    @Test
    func turnOffImmediate() {
        let counter = Counter()
        let sut = makeTestValve(counter: counter, isOn: true)
        #expect(sut.isOn)

        sut.turnOff(duration: 0)
        #expect(.off == sut.state)
        #expect(0 == sut.value)
        #expect(0 == counter.progress)
    }
}
