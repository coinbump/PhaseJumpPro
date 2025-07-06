import Foundation

/// Builds nodes for logging results of parser. Use for debugging
public class LoggerParseBuilder : SomeParseBuilder {
    public class Node: ParseNode {
        public override func evaluate() {
            switch edges.count {
            case 0:
                print("Node is: \(identifier)")
            case 1:
                let leftEdge = edges[0]
                let rightEdge = edges[1]

                // For expressions that require left-right order, we must place an empty
                // node where the left child would be
                print("Node is \(leftEdge.model.type) \(edge.toNode.identifier) \(rightEdge.model.type)")
            default:
                break
            }
        }
    }

    public func node(for token: LexToken) -> ParseNode {
        return .init(kind: "", token: token)
    }
}
