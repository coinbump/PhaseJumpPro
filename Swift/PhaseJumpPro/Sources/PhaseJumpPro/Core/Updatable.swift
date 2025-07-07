import Foundation

/// An object that receives time update events
/// Finish the updatable when it's finished so it can be removed
actor Updatable: SomeUpdatable {
    /// If true, this updatable is finished running and will be removed
    var isFinished: Bool {
        didSet {
            guard isFinished != newValue,
                  isFinished else { return }
            
            onFinishFunc?(self)
        }
    }

    typealias OnUpdateFunc = (Updatable, TimeSlice) -> FinishType
    typealias Func = (Updatable) -> Void
    
    /// (Optional). Called for update events
    var onUpdateFunc: OnUpdateFunc?

    /// (Optional). Called when this finishes
    var onFinishFunc: Func

    init(onUpdateFunc: OnUpdateFunc? = nil) {
        self.onUpdateFunc = onUpdateFunc
    }

    /// Finishes the updatable. After calling this it will not receive any more time updates
    func finish() {
        isFinished = true
    }

    /// @return Updates and returns finish type
    func update(time: TimeSlice) -> FinishType {
        onUpdate(time)
    }

    /// Called for each time delta event
    func onUpdate(time: TimeSlice) {
        guard !isFinished else { return .finish }
        
        let result = onUpdateFunc(*this, time)
        isFinished = result == .finish

        return result
    }
}
