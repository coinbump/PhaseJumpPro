import Foundation

public enum RunType {
    /// Run one time
    case once

    /// Repeats (restarts after finish)
    case `repeat`
}

/// Manages logic for running an updatable that when it finishes, either repeats or finishes
public struct Runner {
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

    public mutating func finish() {
        isFinished = true
    }
    
    public mutating func reset() {
        guard isFinished else { return }
        isFinished = false

        onResetFunc?()
    }
}
