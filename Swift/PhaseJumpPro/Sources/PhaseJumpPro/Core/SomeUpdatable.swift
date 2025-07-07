import Foundation

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 7/6/25
 */

/// Return type for a finishing updatable func
public enum FinishType {
    /// Keep running the updatable
    case `continue`

    /// Complete the updatable
    case finish
}

/// Lightweight updatable, with no stored properties
public protocol SomeUpdatable {
    /// Handle time update event
    func onUpdate(time: TimeSlice) -> FinishType

    /// Allows for cleanup of old updatables
    var isFinished: Bool { get set }
}
