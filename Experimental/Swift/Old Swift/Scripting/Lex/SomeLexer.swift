import Foundation

/*
 RATING: 5 stars
 Simple protocol
 CODE REVIEW: 4/11/22
 */

/// A Lexer converts a string to a stream of tokens
/// Example: x + y becomes IDENTIFIER(value: "x") PLUSOPERATOR IDENTIFIER(value: "y")
public protocol SomeLexer
{
    func setInput(text: String)
    func restartScan()
    func scanToken() -> LexToken?
    func lexAnalyze(text: String) -> [LexToken]
}
