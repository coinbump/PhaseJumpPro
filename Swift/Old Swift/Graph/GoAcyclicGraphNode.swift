/*
 RATING: 5 stars
 Simple interface
 CODE REVIEW: 4/2/22
 */

/// Extends Graph.Node with common Go-type functionality
public class GoAcyclicGraphNode<EdgeModel>: AcyclicGraphNode<EdgeModel> where EdgeModel: Hashable
{
    /// Kickstart some process (optional)
    public func go() { }

    public func isValidTarget(node: AcyclicGraphNode<EdgeModel>) -> Bool {
        false
    }

    /// Apply an operation to the target node (optional)
    public func target(node: AcyclicGraphNode<EdgeModel>) { }

    /// Pick a child, or the next node in a sequence (optional)
    public func selectNext() -> AcyclicGraphNode<EdgeModel>? {
        nil
    }
}
