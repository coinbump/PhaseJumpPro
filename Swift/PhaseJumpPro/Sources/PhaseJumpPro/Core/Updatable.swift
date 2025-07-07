import Foundation

/// An object that receives time update events
/// Finish the updatable when it's finished so it can be removed
public actor Updatable: SomeUpdatable {
    /// If true, this updatable is finished running and will be removed
    public var isFinished: Bool {
        didSet {
            guard isFinished != newValue,
                  isFinished else { return }
            
            onFinishFunc?(self)
        }
    }

    public typealias OnUpdateFunc = (Updatable, TimeSlice) -> FinishType
    public typealias Func = (Updatable) -> Void
    
    /// (Optional). Called for update events
    public var onUpdateFunc: OnUpdateFunc?

    /// (Optional). Called when this finishes
    public var onFinishFunc: Func

    public init(onUpdateFunc: OnUpdateFunc? = nil) {
        self.onUpdateFunc = onUpdateFunc
    }

    /// Finishes the updatable. After calling this it will not receive any more time updates
    public func finish() {
        isFinished = true
    }

    /// @return Updates and returns finish type
    public func update(time: TimeSlice) -> FinishType {
        onUpdate(time)
    }

    /// Called for each time delta event
    public func onUpdate(time: TimeSlice) {
        guard !isFinished else { return .finish }
        
        let result = onUpdateFunc(*this, time)
        isFinished = result == .finish

        return result
    }
}
