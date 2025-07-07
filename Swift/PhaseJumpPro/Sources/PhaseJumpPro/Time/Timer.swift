//import Foundation
//
//public func clamp<T>(_ value: T, minValue: T, maxValue: T) -> T where T : Comparable {
//    return min(max(value, minValue), maxValue)
//}
//
//public actor Timer: SomeUpdatable {
//    public typealias Func = (Timer) -> Void
//    
//    /// Timer duration
//    private var duration: Double
//    
//    /// Time state
//    public var state = 0.0 {
//        didSet {
//            guard state != oldValue else { return }
//            state = clamp(state, minValue: 0.0, maxValue: duration)
//            runner.isFinished = state >= duration
//        }
//    }
//    
//    private var runner: Runner
//    
//    /// (Optional). Called when this finishes
//    var onFinishFunc: Func?
//    
//    init(_ duration: Double, runType: RunType)  {
//        self.duration = duration
//        self.runner = .init(runType: runType)
//   
//        runner.onFinishFunc = { self.onFinish() }
//        runner.onResetFunc = { self.onReset() }
//    }
//    
//    func stop() {
//        runner.runType = .once
//        runner.isFinished = true
//    }
//    
//    func reset() {
//        runner.reset()
//    }
//    
//    func onReset() {
//        state = 0
//    }
//    
//    var progress: Double {
//        clamp(state / duration, minValue: 0.0, maxValue: 1.0)
//    }
//    
//    func setProgress(_ progress: Double) {
//        let progress = clamp(progress, minValue: 0.0, maxValue: 1.0)
//        state = self.progress * duration
//    }
//    
//    func onFinish() {
//        onFinishFunc?(self)
//    }
//    
//    // MARK: SomeUpdatable
//    
//    public func onUpdate(time: TimeSlice) -> FinishType {
//        if isFinished || duration <= 0 {
//            return .finish
//        }
//        
//        let delta = timeDelta(for: time)
//        if delta <= 0 {
//            return .continue
//        }
//        
//        // We can't break down large deltas here because events will get compressed
//        // Example: an event happens every 3 seconds, we get a 9 second delta,
//        // they should be 3 seconds apart but are 0 seconds apart instead
//        state = state + delta
//        
//        return isFinished ? .finish : .continue
//    }
//    
//    func timeDelta(for time: TimeSlice) -> Double {
//        time.delta
//    }
//    
//    /// Allows for cleanup of old updatables
//    public var isFinished: Bool {
//        get {
//            runner.isFinished
//        }
//        set {
//            runner.isFinished = newValue
//        }
//    }
//}
