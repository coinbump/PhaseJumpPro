import Foundation

/// A binary tree has a left and right child
public class BinaryTreeNode<EdgeModel>: AcyclicGraphNode<EdgeModel> where EdgeModel: Hashable
{
    let edgeModelFactory: () -> EdgeModel

    var leftChild: BinaryTreeNode<EdgeModel>?
    {
        willSet {
            guard newValue != leftChild else { return }
            guard let leftChild = leftChild else { return }
            removeEdges(to: leftChild)
        }
        didSet {
            guard let leftChild = leftChild else {
                return
            }

            add(model: edgeModelFactory(), toNode: leftChild)
        }
    }

    var rightChild: BinaryTreeNode<EdgeModel>?
    {
        willSet {
            guard newValue != rightChild else { return }
            guard let rightChild = rightChild else { return }
            removeEdges(to: rightChild)
        }
        didSet {
            guard let rightChild = rightChild else {
                return
            }

            add(model: edgeModelFactory(), toNode: rightChild)
        }
    }

    public init(edgeModelFactory: @escaping () -> EdgeModel) {
        self.edgeModelFactory = edgeModelFactory
    }
}
