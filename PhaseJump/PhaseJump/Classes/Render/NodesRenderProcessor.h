#pragma once

#include "ComponentsHandler.h"
#include "NodeHandler.h"
#include "RenderProcessor.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/14/25
 */
namespace PJ {
    /// Signal sent by render processor to nodes that use immediate render
    struct NodesRenderSignalCore {
    public:
        RenderProcessor::Phase phase;
    };

    using NodesRenderSignal = Signal<NodesRenderSignalCore>;

    /// Sends a signal to each node during the render processor phase
    /// Example: a node that does its own imGUI painting
    class NodesRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;
        using This = NodesRenderProcessor;
        using NodeHandler = PJ::NodeHandler<RenderProcessor::Phase>;

        struct Config {
            String signalId;
        };

        String signalId;

        NodesRenderProcessor(Config const& config, RenderProcessor::Config baseConfig);
    };
} // namespace PJ
