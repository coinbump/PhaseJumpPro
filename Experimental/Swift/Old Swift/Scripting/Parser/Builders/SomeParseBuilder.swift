import Foundation

/// Builds a parse tree based on information received from the parser
public protocol SomeParseBuilder {
    func node(for token: LexToken) -> ParseNode
}
