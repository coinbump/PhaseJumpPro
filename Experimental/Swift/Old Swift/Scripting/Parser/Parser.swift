import Foundation

/// A simple parser, for simple languages
/// FUTURE: handle more complex languages if needed
public class Parser: SomeParser {
    private var stack = [LexToken]()
    private var nextToken: LexToken?

    public func parse(text: String, lexer: SomeLexer, builder: SomeParseBuilder) -> ParseNode
    {
        let grammar = ParseGrammar(ruleGroups: [])
        lexer.restartScan()

        var result = ParseNode(kind: "", token: .init(kind: .name("hello"), text: ""))

        for ruleGroup in grammar.ruleGroups {
            for rule in ruleGroup.rules {
                for pattern in rule.patterns {
                    if pattern.count == 1 {
                        let patternToken = pattern[0]
                        guard let lexToken = lexer.scanToken() else { return result }

                        if patternToken == lexToken {
                            return builder.node(for: patternToken)
                        }
                    }
                    else if pattern.count == 2 {
                        let patternToken1 = pattern[0]
                        let patternToken2 = pattern[1]
                        // TODO: what about stack?
                    }
                }
            }
        }

        return result
    }
}
