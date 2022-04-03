import Foundation

/*
 RATING: 5 stars
 Simple class
 CODE REVIEW: 4/2/22
 */
/// Encapsulates time delta for update events
/// Allows for future time delta logic vs a standard float
public struct TimeSlice
{
    public let delta: Double

    public init(delta: Double) {
        self.delta = delta
    }
}
