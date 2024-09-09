#include "DepthFirstOrderRenderProcessor.h"
#include "RenderWorldSystem.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

using OrderMap = UnorderedMap<String, uint32_t>;

void Paint(OrderMap& orderMap, WorldNode& node, uint32_t& order) {
    auto nodeId = MakeString(node.IntId());
    orderMap.insert_or_assign(nodeId, order);
    order++;

    for (auto& child : node.Children()) {
        Paint(orderMap, *child, order);
    }
};

void DepthFirstOrderRenderProcessor::Process(RenderSystemModel& systemModel) {
    GUARD(systemModel.root)

    /// Object render id to order map. For Z-ordering
    OrderMap orderMap;

    uint32_t order = 0;

    Paint(orderMap, *systemModel.root, order);

    /// Paint render models (there will be gaps in order because of nodes in the tree that don't
    /// have a renderer)
    std::for_each(systemModel.models.begin(), systemModel.models.end(), [&](RenderModel& model) {
        try {
            model.order = orderMap.at(model.id);
        } catch (...) {
            model.order = 0;
        }
    });

    std::sort(systemModel.models.begin(), systemModel.models.end(), systemModel.modelSortFunc);
}
