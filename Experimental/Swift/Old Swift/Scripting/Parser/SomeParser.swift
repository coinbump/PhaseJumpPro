import Foundation

/// Interprets a stream of tokens given to it by a lexical analyzer
public protocol SomeParser {
    /// Parse tokens and return the root of a parse tree (or interpret them while matching)
    // TODO: rename text to buffer?
    func parse(text: String, lexer: SomeLexer, builder: SomeParseBuilder) -> ParseNode
}
