import Foundation

public enum class RunType {
    /// Run one time
    case once

    /// Repeats (restarts after finish)
    case `repeat`
}

/// Manages logic for running an updatable that when it finishes, either repeats or finishes
public actor Runner {
    typealias OnResetFunc = (Self) -> Void
    typealias OnFinishFunc = (Self) -> Void

    private var runType: RunType

    public OnResetFunc onResetFunc;
    public OnFinishFunc onFinishFunc;

    /// If true, we are finished running
    public var isFinished = false {
        didSet {
            guard newValue != oldValue, isFinished else { return }
            
            onFinishFunc(self)

            switch runType {
            case .repeat:
                Reset()
            default:
                break
            }
        }
    }

public:
    public init(runType: RunType) {
        self.runType = runType
    }

    func Reset() {
        guard isFinished else { return }
        isFinished = false

        onResetFunc(self)
    }
};
