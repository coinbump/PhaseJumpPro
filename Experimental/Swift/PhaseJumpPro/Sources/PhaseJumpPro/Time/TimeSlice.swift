import Foundation

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/6/25
 */

/// Encapsulates time delta for update events
/// Allows for future time delta logic vs a standard float
public struct TimeSlice: Sendable
{
    public let delta: Double

    public init(delta: Double) {
        self.delta = delta
    }
    
    public static func *(lhs: Self, rhs: Self) -> Self {
        .init(delta: lhs.delta * rhs.delta)
    }
}
