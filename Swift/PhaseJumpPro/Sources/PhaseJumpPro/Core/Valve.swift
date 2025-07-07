import Foundation

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/7/25
 */

/**
 Common design pattern for interactive media
 A valve transitions between two states over time, usually with an animation
 Example: a switch, or a water valve that opens and closes
 If you need more complex animations (time on hold, time off hold), use the driver pattern to
 drive the valve's states
 */
public final class Valve: SomeUpdatable {
    public typealias OnValveUpdateFunc = (Valve) -> Void
    
    public enum StateType {
        /// Valve is off
        case off

        /// Valve is turning on
        case turnOn

        /// Valve is on
        case on

        /// Valve is turning off
        case turnOff
    }

    private(set) var state: StateType = .off {
        didSet {
            guard state != oldValue else { return }
            onValveUpdate()
        }
    }
    
    public var onValveUpdateFunc: OnValveUpdateFunc? {
        didSet {
            onValveUpdate()
        }
    }
    
    private var timer: Timer?
    private let updatable = Updatable()
    
    public init(isOn: Bool = false) {
        state = isOn ? .on : .off
        
        updatable.onUpdateFunc = { [weak self] (time: TimeSlice) -> FinishType in
            guard let self else { return .finish }
            guard let timer else { return .continue }
            
            timer.onUpdate(time: time)

            if timer.isFinished {
                timer.reset()

                switch state {
                case .turnOn:
                    state = .on
                    break
                case .turnOff:
                    state = .off
                    break
                default:
                    print("ERROR. Animating valve with no destination")
                    break
                }
            }

            // Even though valve state hasn't changed, the time state has
            onValveUpdate()

            return .continue
        }
    }

    private var timerProgress: Double {
        timer?.progress ?? 0
    }
    
    public var value: Double {
        switch state {
        case .on:
            1
        case .off:
            0
        case .turnOn:
            timerProgress
        case .turnOff:
            1.0 - timerProgress
        }
    }

    /**
     Turn on the valve in N seconds
     If we are interrupting a previous animation, the actual duration will be proportional to
     the distance left
     */
    public func turnOn(duration: Double) {
        guard !isOn else { return }
        
        if duration <= 0 {
            state = .on
            timer?.reset()
            return
        }

        let value = self.value

        state = .turnOn
        let timer = Timer(duration, runType: .once)
        timer.progress = value
        self.timer = timer
    }

    /**
     Turn on the valve in N seconds
     If we are interrupting a previous animation, the actual duration will be proportional to
     the distance left
     */
    public func turnOff(duration: Double) {
        guard !isOff else { return }

        if duration <= 0 {
            state = .off
            timer?.reset()
            return
        }

        let value = self.value

        state = .turnOff
        let timer = Timer(duration, runType: .once)
        timer.progress = 1.0 - value
        self.timer = timer
    }

    public var isOn: Bool {
        state == .on
    }

    public var isOff: Bool {
        state == .off
    }
    
    public func onValveUpdate() {
        onValveUpdateFunc?(self)
    }
    
    // MARK: SomeUpdatable
    
    @discardableResult
    public func onUpdate(time: TimeSlice) -> FinishType {
        updatable.onUpdate(time: time)
    }
    
    public var isFinished: Bool {
        get {
            updatable.isFinished
        }
        set {
            updatable.isFinished = newValue
        }
    }
}
