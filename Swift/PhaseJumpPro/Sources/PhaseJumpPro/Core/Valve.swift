//import Foundation
//
//// TODO: finish
//#if false
//
///**
// Common design pattern for interactive media
// A valve transitions between two states over time, usually with an animation
// Example: a switch, or a water valve that opens and closes
// If you need more complex animations (time on hold, time off hold), use the driver pattern to
// drive the valve's states
// */
//public actor Valve: SomeUpdatable {
//    public typealias OnValveUpdateFunc = (Valve) -> Void
//
//    public enum StateType {
//        /// Valve is off
//        case off
//
//        /// Valve is turning on
//        case turnOn
//
//        /// Valve is on
//        case on
//
//        /// Valve is turning off
//        case turnOff
//    }
//
//    private(set) var state: StateType = .off {
//        didSet {
//            GUARD(state != value)
//            state = value
//
//            onValveUpdate()
//        }
//    }
//    private var timer: Timer
//    
//    public var onValveUpdateFunc: OnValveUpdateFunc {
//        didSet {
//            onValveUpdate()
//        }
//    }
//
//    init(isOn: Bool = false) {
//        state = isOn ? .on : .off
//        
//        onUpdateFunc = { updatable: SomeUpdatable, time: TimeSlice in
//            guard let valve = updatable as? Self else {
//                return .continue
//            }
//            
//            valve.timer.onUpdate(time)
//
//            if valve.timer.isFinished {
//                valve.timer.reset()
//
//                switch valve.state {
//                case .turnOn:
//                    valve.state = .on
//                    break
//                case .turnOff:
//                    valve.state = .off
//                    break
//                default:
//                    print("ERROR. Animating valve with no destination")
//                    break
//                }
//            }
//
//            // Even though valve state hasn't changed, the time state has
//            valve.onValveUpdate()
//
//            return FinishType::Continue
//        }
//    }
//
//    var value: Double {
//        switch state {
//        case .on:
//            1
//        case .off:
//            0
//        case .turnOn:
//            timer.Progress()
//        case .turnOff:
//            1.0f - timer.Progress()
//        }
//    }
//
//    /**
//     Turn on the valve in N seconds
//     If we are interrupting a previous animation, the actual duration will be proportional to
//     the distance left
//     */
//    func turnOn(duration: Double) {
//        guard !isOn else { return }
//        
//        if duration <= 0 {
//            state = .on
//            timer.reset()
//            return
//        }
//
//        let value = Value()
//
//        state = .turnOn
//        timer = .init(duration, runType: .once)
//        timer.progress = value
//    }
//
//    /**
//     Turn on the valve in N seconds
//     If we are interrupting a previous animation, the actual duration will be proportional to
//     the distance left
//     */
//    func turnOff(duration: Double) {
//        guard !isOff else { return }
//
//        if duration <= 0 {
//            state = .off
//            timer.reset()
//            return
//        }
//
//        let value = Value()
//
//        state = .turnOff
//        timer = .init(duration, runType: .once)
//        timer.progress = 1.0f - value
//    }
//
//    var isOn: Bool {
//        state == .on
//    }
//
//    var isOff: Bool {
//        state == .off
//    }
//    
//    func onValveUpdate() {
//        onValveUpdateFunc?(self)
//    }
//}
//#endif
