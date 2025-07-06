import Foundation

/*
 RATING: 5 stars
 Has united tests
 CODE REVIEW: 4/11/22
 */

/// Converts a string to a stream of tokens via rules in LexGrammar
public class Lexer: SomeLexer
{
    /// Symbols are stored to check for conflict resolution later
    private class Symbol: Hashable
    {
        let token: LexToken
        let matchRange: Range<String.Index>
        var isDisabled = false

        init(token: LexToken, matchRange: Range<String.Index>)
        {
            self.token = token
            self.matchRange = matchRange
        }

        func hash(into hasher: inout Hasher) {
            hasher.combine(token)
            hasher.combine(matchRange)
        }

        static func == (lhs: Lexer.Symbol, rhs: Lexer.Symbol) -> Bool {
            lhs.token == rhs.token && lhs.matchRange == rhs.matchRange
        }
    }

    /// Grammar defines regular expression rules that match to a token
    /// Example: [a-zA-z]+ -> Identifier
    let grammar: LexGrammar

    public var text: String {
        didSet {
            restartScan()
        }
    }

    // MARK: - Private

    private var scanIndex: String.Index?

    // MARK: - Methods

    public init(grammar: LexGrammar, text: String) {
        self.grammar = grammar
        self.text = text
    }

    public func setInput(text: String)
    {
        self.text = text
    }

    /// Scan the next token in the text
    public func scanToken() -> LexToken?
    {
        guard let scanIndex = scanIndex else {
            print("Missing scan index")
            return nil
        }

        var unsortedSymbols = [Symbol]()

        let options: NSRegularExpression.Options = grammar.options.contains(.caseInsensitive) ? [.caseInsensitive] : []
        let nsText = NSString(string: text)

        // Skip whitespace
        var whitespaceIndex = scanIndex
        while text[whitespaceIndex].isWhitespace {
            let char = text[whitespaceIndex]
            whitespaceIndex = text.index(scanIndex, offsetBy: 1)

            if char.isNewline {
                self.scanIndex = whitespaceIndex
                return LexToken(kind: .newline, text: "")
            }

            if whitespaceIndex >= text.endIndex {
                return nil
            }
        }

        let startIndex = whitespaceIndex

        for rule in grammar.rules {
            do {
                let regex = try NSRegularExpression(pattern: rule.pattern, options: options)

                let nsrange = NSRange(startIndex..<text.endIndex, in: text)

                if let match = regex.firstMatch(in: text, options: [.anchored], range: nsrange) {

                    let matchString = nsText.substring(with: match.range)

                    let token = LexToken(kind: .name(rule.identifier), text: matchString)

                    // Keep symbols for conflict resolution later
                    if let range = Range<String.Index>(match.range, in: text) {
                        let symbol = Symbol(token: token, matchRange: range)
                        unsortedSymbols.append(symbol)
                    }
                }
            }
            catch let error {
                print("Error. Regex error: \(error.localizedDescription)")
            }
        }

        if (unsortedSymbols.isEmpty) {
           // No grep match, send individual characters
           self.scanIndex = text.index(startIndex, offsetBy: 1)
           let char = text[startIndex]
           return LexToken(kind: .character(char), text: String(char))
        }

        let symbols = unsortedSymbols.sorted(by: { $0.matchRange.lowerBound < $1.matchRange.lowerBound })

        // Produce a list of tokens and do conflict resolution if needed
        var symbolsSet = Set<Symbol>(symbols)
        for symbol in symbols {
            // Disable symbols instead of mutating the list as we're iterating
            if symbol.isDisabled { continue; }

            symbolsSet.remove(symbol)

            let conflicts = symbolsSet.filter { $0.matchRange.overlaps(symbol.matchRange) }
            if conflicts.isEmpty {
                self.scanIndex = symbol.matchRange.upperBound
                return symbol.token
            }
            // When conflicts occur, choose the longest match
            // Example "a" takes precedence over a, if a quote rule exists in the grammar
            // FUTURE: support precedence priority if needed
            else {
                var conflictsArray = Array(conflicts)
                conflictsArray.append(symbol)
                let sortedConflicts = conflictsArray.sorted(by: { text.distance(from: $0.matchRange.lowerBound, to: $0.matchRange.upperBound) > text.distance(from: $1.matchRange.lowerBound, to: $1.matchRange.upperBound) })

                let newSymbol = sortedConflicts[0];
                self.scanIndex = newSymbol.matchRange.upperBound
                return newSymbol.token
            }
        }

        return nil
    }

    public func lexAnalyze(text: String) -> [LexToken]
    {
        self.text = text
        var result = [LexToken]()

        restartScan()

        var thisScanIndex = text.startIndex
        while thisScanIndex < text.endIndex {
            if let token = scanToken() {
                result.append(token)
            } else {
                return result;
            }

            thisScanIndex = scanIndex ?? text.endIndex
        }

        return result
    }

    public func restartScan()
    {
        scanIndex = text.startIndex
    }
}
