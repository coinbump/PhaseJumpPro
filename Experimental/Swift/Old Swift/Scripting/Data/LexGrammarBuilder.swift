import Foundation

public struct LexGrammarBuilder
{
    public func decode(from data: Data) -> LexGrammar?
    {
        do {
            let grammarDecodable = try JSONDecoder().decode(LexGrammarDecodable.self, from: data)

            let rules: [LexRule] = grammarDecodable.rules.map { .init(pattern: $0.pattern, identifier: $0.id) }
            let result = LexGrammar(rules: rules, options: (grammarDecodable.caseSensitive ?? true) ? [] : [.caseInsensitive])
            return result
        }
        catch let error {
            print("Error. Decode error: \(error.localizedDescription)")
            return LexGrammar(rules: [], options: [])
        }
    }
}
