import Foundation

public struct ParseRule
{
    public typealias Pattern = [LexToken]

    /// A pattern looks something like this: sentence: noun verb adverb | noun verb
    public let patterns: [Pattern]

    /// Create a node for the parse tree and send it back
    public let onMatch: (() -> ParseNode)?
}

public struct ParseRuleGroup
{
    /// List of rules
    public let rules: [ParseRule]
}
