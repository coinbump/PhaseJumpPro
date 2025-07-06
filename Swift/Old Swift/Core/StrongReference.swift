import Foundation

/*
 RATING: 5 stars
 Simple wrapper
 CODE REVIEW: 4/11/22
 */

public class StrongReference<T> where T: AnyObject {
    var strongValue: T

    init (value: T) {
        self.strongValue = value
    }
}

// MARK: - SomeReference

extension StrongReference: SomeReference
{
    public var value: T? {
        strongValue
    }
}

// MARK: - Equatable

extension StrongReference: Equatable where T: Equatable {
    public static func == (lhs: StrongReference, rhs: StrongReference) -> Bool {
        return lhs.strongValue == rhs.strongValue
    }
}

// MARK: - Hashable

extension StrongReference: Hashable where T: Hashable {
    public func hash(into hasher: inout Hasher) {
        strongValue.hash(into: &hasher)
    }
}
