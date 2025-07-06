import Foundation

public struct LexGrammarDecodable : Decodable
{
    public let caseSensitive: Bool?
    public let rules: [LexGrammarRuleDecodable]
}

public struct LexGrammarRuleDecodable : Decodable
{
    let pattern: String
    let id: String
}
