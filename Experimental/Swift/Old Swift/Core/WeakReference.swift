import Foundation

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/2/22
 */

/// Use for collections of weak references
public class WeakReference<T> where T: AnyObject {
    weak var weakValue: T?

    init (value: T) {
        self.weakValue = value
    }
}

// MARK: - SomeReference

extension WeakReference: SomeReference
{
    public var value: T? {
        weakValue
    }
}

// MARK: - Equatable

extension WeakReference: Equatable where T: Equatable {
    public static func == (lhs: WeakReference, rhs: WeakReference) -> Bool {
        return lhs.weakValue == rhs.weakValue
    }
}

// MARK: - Hashable

extension WeakReference: Hashable where T: Hashable {
    public func hash(into hasher: inout Hasher) {
        weakValue.hash(into: &hasher)
    }
}
