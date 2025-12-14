import Foundation

/*
 RATING: 5 stars
 Simple wrapper
 CODE REVIEW: 4/11/22
 */

/// Allows us to abstract a weak or strong reference based on use
/// Example: cyclic vs acyclic graph
public protocol SomeReference {
    associatedtype Value

    var value: Value? { get }
}
