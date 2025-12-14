import Foundation

/*
 RATING: 5 stars
 Simple struct
 CODE REVIEW: 4/2/22
 */
public struct GraphStandardEdgeModel
{
    public let identifier: String // Example: north, west, left, right
    public let type: String // Example: input, output
    public let weight: Double
    public let tags: Tags

    public init(
        identifier: String,
        type: String = "",
        weight: Double = 1.0,
        tags: Tags = Tags())
    {
        self.identifier = identifier
        self.type = type
        self.weight = weight
        self.tags = tags
    }
}

// MARK: - Equatable

extension GraphStandardEdgeModel: Equatable {
    public static func == (lhs: GraphStandardEdgeModel, rhs: GraphStandardEdgeModel) -> Bool {
        return lhs.identifier == rhs.identifier
    }
}

// MARK: - Hashable

extension GraphStandardEdgeModel: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(identifier)
    }
}
