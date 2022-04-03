/*
 RATING: 5 stars
 Simple interface
 CODE REVIEW: 4/2/22
 */
/// Extends Graph.Node with common Go-type functionality
public class GoGraphNode<EdgeModel>: GraphNode<EdgeModel> where EdgeModel: Hashable
{
    /// Kickstart some process (optional)
    public func go() { }

    public func isValidTarget(node: GraphNode<EdgeModel>) -> Bool {
        false
    }

    /// Apply an operation to the target node (optional)
    public func target(node: GraphNode<EdgeModel>) { }

    /// Pick a child, or the next node in a sequence (optional)
    public func selectNext() -> GraphNode<EdgeModel>? {
        nil
    }
}
