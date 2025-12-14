#pragma once

#include "ComponentsHandler.h"
#include "NodeHandler.h"
#include "RenderProcessor.h"

// TODO: code review + tests
namespace PJ {
    /// Allows each node to be processed during the render processor phase
    /// Example: a node that does its own imGUI painting
    class NodesRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;
        using This = NodesRenderProcessor;
        using NodeHandler = PJ::NodeHandler<RenderProcessor::Phase>;

        struct Config {
            String type;
        };

        String type;

        NodesRenderProcessor(Config config, RenderProcessor::Config baseConfig);
    };
} // namespace PJ
