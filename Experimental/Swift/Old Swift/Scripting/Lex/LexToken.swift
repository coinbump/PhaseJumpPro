import Foundation

/*
 RATING: 5 stars
 Simple struct
 CODE REVIEW: 4/11/22
 */

/// Token returned by Lexer. Used by parser to interpret syntax
public struct LexToken: Hashable
{
    public enum Kind: Hashable {
        case name(String)
        case character(Character)
        case newline
    }

    /// The unique kind of token this is
    public let kind: Kind

    /// The string from the input buffer
    public let text: String
}
