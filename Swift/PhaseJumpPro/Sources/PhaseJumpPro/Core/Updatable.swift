/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/7/25
 */

/// An object that receives time update events
/// Finish the updatable when it's finished so it can be removed
public final class Updatable: SomeUpdatable {
    public typealias OnUpdateFunc = (TimeSlice) -> FinishType
    public typealias Func = () -> Void
    
    /// If true, this updatable is finished running and will be removed
    public var isFinished: Bool = false {
        didSet {
            guard isFinished != oldValue,
                  isFinished else { return }
            
            onFinishFunc?()
        }
    }
    
    /// (Optional). Called for update events
    public var onUpdateFunc: OnUpdateFunc?

    /// (Optional). Called when this finishes
    public var onFinishFunc: Func?

    public init(
        onUpdateFunc: OnUpdateFunc? = nil,
        onFinishFunc: Func? = nil
    ) {
        self.onUpdateFunc = onUpdateFunc
        self.onFinishFunc = onFinishFunc
    }

    /// Finishes the updatable. After calling this it will not receive any more time updates
    public func finish() {
        isFinished = true
    }

    /// Called for each time delta event
    @discardableResult
    public func onUpdate(time: TimeSlice) -> FinishType {
        guard !isFinished else { return .finish }
        
        let result = onUpdateFunc?(time) ?? .continue
        isFinished = result == .finish

        return result
    }
}
