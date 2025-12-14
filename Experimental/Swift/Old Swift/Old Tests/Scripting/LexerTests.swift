import XCTest
@testable import PhaseJumpPro

final class LexerTests: XCTestCase {

    func test_caseInsensitive() {
        let grammar = LexGrammar(rules: [LexRule(pattern: "[a-z]+", identifier: "id")], options: [.caseInsensitive])
        let lexer = Lexer(grammar: grammar, text: "")

        let tokens = lexer.lexAnalyze(text: "arizona ice tea")
        let tokenKinds = tokens.map { $0.kind }
        let tokenValues = tokens.map { $0.text }
        XCTAssertEqual(tokenKinds, [.name("id"), .name("id"), .name("id")])
        XCTAssertEqual(tokenValues, ["arizona", "ice", "tea"])
    }

    func test_caseSensitive() {
        let grammar = LexGrammar(rules: [LexRule(pattern: "[a-z]+", identifier: "id")], options: [])
        let lexer = Lexer(grammar: grammar, text: "")

        let tokens = lexer.lexAnalyze(text: "arizona I tea")
        let tokenKinds = tokens.map { $0.kind }
        let tokenValues = tokens.map { $0.text }
        XCTAssertEqual(tokenKinds, [.name("id"), .character("I"), .name("id")])
        XCTAssertEqual(tokenValues, ["arizona", "I", "tea"])
    }

    func test_caseSensitiveMixed() {
        let grammar = LexGrammar(rules: [LexRule(pattern: "[a-z]+", identifier: "id")], options: [])
        let lexer = Lexer(grammar: grammar, text: "")

        let tokens = lexer.lexAnalyze(text: "arizona Ic tea")
        let tokenKinds = tokens.map { $0.kind }
        let tokenValues = tokens.map { $0.text }
        XCTAssertEqual(tokenKinds, [.name("id"), .character("I"), .name("id"), .name("id")])
        XCTAssertEqual(tokenValues, ["arizona", "I", "c", "tea"])
    }

    func test_conflictResolution_single() {
        let rules: [LexRule] = [
            LexRule(pattern: "[a-z]+", identifier: "id"),
            LexRule(pattern: "'[a-z]+'", identifier: "quote")
        ]
        let grammar = LexGrammar(rules: rules, options: [])
        let lexer = Lexer(grammar: grammar, text: "")

        let tokens = lexer.lexAnalyze(text: "arizona 'arizona'")
        let tokenKinds = tokens.map { $0.kind }
        let tokenValues = tokens.map { $0.text }
        XCTAssertEqual(tokenKinds, [.name("id"), .name("quote")])
        XCTAssertEqual(tokenValues, ["arizona", "'arizona'"])
    }

    func test_regexNumberQuotedLetters() {
        let rules: [LexRule] = [
            LexRule(pattern: "[0-9]+'[a-z]+'", identifier: "sq")
        ]

        let grammar = LexGrammar(rules: rules, options: [])
        let lexer = Lexer(grammar: grammar, text: "")

        let tokens = lexer.lexAnalyze(text: "arizona 'arizona' 3'arizona'").filter { $0.kind == .name("sq") }
        let tokenKinds = tokens.map { $0.kind }
        let tokenValues = tokens.map { $0.text }
        XCTAssertEqual(tokenKinds, [.name("sq")])
        XCTAssertEqual(tokenValues, ["3'arizona'"])
    }

    func test_conflictResolution_multiple() {
        let rules: [LexRule] = [
            LexRule(pattern: "[a-z]+", identifier: "id"),
            LexRule(pattern: "'[a-z]+'", identifier: "quote"),
            LexRule(pattern: "[0-9]+'[a-z]+'", identifier: "sq")
        ]

        let grammar = LexGrammar(rules: rules, options: [])
        let lexer = Lexer(grammar: grammar, text: "")

        let tokens = lexer.lexAnalyze(text: "arizona 'arizona' 3'arizona'")
        let tokenKinds = tokens.map { $0.kind }
        let tokenValues = tokens.map { $0.text }
        XCTAssertEqual(tokenKinds, [.name("id"), .name("quote"), .name("sq")])
        XCTAssertEqual(tokenValues, ["arizona", "'arizona'", "3'arizona'"])
    }

    func test_noMatches() {
        let rules: [LexRule] = [
            LexRule(pattern: "[a-z]+", identifier: "id"),
            LexRule(pattern: "'[a-z]+'", identifier: "quote")
        ]

        let grammar = LexGrammar(rules: rules, options: [])
        let lexer = Lexer(grammar: grammar, text: "")

        let tokens = lexer.lexAnalyze(text: "041!34").filter { $0.kind == .name("id") || $0.kind == .name("quote") }
        let tokenKinds = tokens.map { $0.kind }
        let tokenValues = tokens.map { $0.text }
        XCTAssertEqual(tokenKinds, [])
        XCTAssertEqual(tokenValues, [])
    }

    func test_unicodeString() {
        let rules: [LexRule] = [
            LexRule(pattern: ".*", identifier: "id")
        ]

        let grammar = LexGrammar(rules: rules, options: [])
        let lexer = Lexer(grammar: grammar, text: "")

        let unicodeString = "04🤔04"
        let tokens = lexer.lexAnalyze(text: unicodeString)
        let tokenKinds = tokens.map { $0.kind }
        let tokenValues = tokens.map { $0.text }
        XCTAssertEqual(tokenKinds, [.name("id")])
        XCTAssertEqual(tokenValues, [unicodeString])
    }

    func test_newline() {
        let grammar = LexGrammar(rules: [LexRule(pattern: "[a-z]+", identifier: "id")], options: [.caseInsensitive])
        let lexer = Lexer(grammar: grammar, text: "")

        let tokens = lexer.lexAnalyze(text: "arizona\nice tea")
        let tokenKinds = tokens.map { $0.kind }
        let tokenValues = tokens.map { $0.text }
        XCTAssertEqual(tokenKinds, [.name("id"), .newline, .name("id"), .name("id")])
        XCTAssertEqual(tokenValues, ["arizona", "", "ice", "tea"])
    }
}
