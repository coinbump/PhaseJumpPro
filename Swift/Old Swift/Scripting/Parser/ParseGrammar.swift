import Foundation

/// Parse grammar defines rules to build syntax tree
/// For complex languages, you might achieve better results with a handwritten parser
public struct ParseGrammar
{
    let ruleGroups: [ParseRuleGroup]
}
