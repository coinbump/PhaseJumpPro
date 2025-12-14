import Foundation

/// Each node from the parse tree root is a statement. Each statement is a combination of expressions and tokens
public class ParseNode: AcyclicGraphNode<GraphStandardEdgeModel>
{
    let kind: String
    let token: LexToken

    public init(kind: String, token: LexToken) {
        self.kind = kind
        self.token = token

        super.init(identifier: kind)
    }

    public func evaluate() {
    }
}
