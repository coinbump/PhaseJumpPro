//
//  GraphNodeDecodable.swift
//  pj-swift-example-dialogue
//
//  Created by Jeremy Vineyard on 4/2/22.
//

import Foundation

struct GraphNodesDecodable: Decodable
{
    let nodes: [GraphNodeDecodable]
}

struct GraphNodeDecodable: Decodable
{
    let id: String
    let text: String
    let edges: [GraphNodeEdgeDecodable]?
}

struct GraphNodeEdgeDecodable: Decodable
{
    let id: String
    let title: String
}
