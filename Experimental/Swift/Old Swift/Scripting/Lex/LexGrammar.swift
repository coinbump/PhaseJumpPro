import Foundation

/*
 RATING: 5 stars
 Simple struct
 CODE REVIEW: 4/11/22
 */

/// Defines a set of rules for converting strings into a stream of Lex tokens
/// Example: x + y becomes IDENTIFIER(value: "x") PLUSOPERATOR IDENTIFIER(value: "y")
public struct LexGrammar
{
    public struct Options: OptionSet {
        public let rawValue: Int

        public init(rawValue: Int) {
            self.rawValue = rawValue
        }

        /// Ignores case when matching rules
        public static let caseInsensitive = Options(rawValue: 1 << 0)

        public static let all: Options = [.caseInsensitive]
    }

    let rules: [LexRule]
    let options: Options

    public init(rules: [LexRule], options: Options) {
        self.rules = rules
        self.options = options
    }
}
