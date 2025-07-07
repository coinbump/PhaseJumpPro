import Foundation

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/25
 */

public enum RunType {
    /// Run one time
    case once

    /// Repeats (restarts after finish)
    case `repeat`
}

/// Manages logic for running an updatable that when it finishes, either repeats or finishes
public final class Runner {
    public typealias OnResetFunc = () -> Void
    public typealias OnFinishFunc = () -> Void

    public var runType: RunType

    public var onResetFunc: OnResetFunc?
    public var onFinishFunc: OnFinishFunc?

    /// If true, we are finished running
    public var isFinished = false {
        didSet {
            guard isFinished != oldValue, isFinished else { return }
            
            onFinishFunc?()

            switch runType {
            case .repeat:
                reset()
            default:
                break
            }
        }
    }
    
    public init(runType: RunType) {
        self.runType = runType
    }

    public func finish() {
        isFinished = true
    }
    
    public func reset() {
        guard isFinished else { return }
        isFinished = false

        onResetFunc?()
    }
}
