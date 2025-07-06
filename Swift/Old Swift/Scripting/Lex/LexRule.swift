import Foundation

/*
 RATING: 5 stars
 Simple struct
 CODE REVIEW: 4/11/22
 */

/// Defines a pattern that when matched, is sent as a token in the output stream
/// When two rules conflict, the longest match is used
public struct LexRule
{
    /// Regular expression pattern to be matched. Example: ([a-zA-Z]+)
    public let pattern: String

    /// Token identifier
    public let identifier: String
}
