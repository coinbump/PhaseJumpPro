import Foundation

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/2/22
 */
/// Use for collections of weak references
public class WeakReference<T> where T: AnyObject {
    weak var value: T?

    init (value: T) {
        self.value = value
    }
}

// MARK: - Equatable

extension WeakReference: Equatable where T: Equatable {
    public static func == (lhs: WeakReference, rhs: WeakReference) -> Bool {
        return lhs.value == rhs.value
    }
}

// MARK: - Hashable

extension WeakReference: Hashable where T: Hashable {
    public func hash(into hasher: inout Hasher) {
        value.hash(into: &hasher)
    }
}
