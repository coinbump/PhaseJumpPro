import Foundation

public enum class RunType {
    /// Run one time
    case once

    /// Repeats (restarts after finish)
    case `repeat`
}

/// Manages logic for running an updatable that when it finishes, either repeats or finishes
public struct Runner {
    typealias OnResetFunc = () -> Void
    typealias OnFinishFunc = () -> Void

    private var runType: RunType

    public OnResetFunc onResetFunc
    public OnFinishFunc onFinishFunc

    /// If true, we are finished running
    public var isFinished = false {
        didSet {
            guard newValue != oldValue, isFinished else { return }
            
            onFinishFunc()

            switch runType {
            case .repeat:
                reset()
            default:
                break
            }
        }
    }

public:
    public init(runType: RunType) {
        self.runType = runType
    }

    func reset() {
        guard isFinished else { return }
        isFinished = false

        onResetFunc()
    }
}
