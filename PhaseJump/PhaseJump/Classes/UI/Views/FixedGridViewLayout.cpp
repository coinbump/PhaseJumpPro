#include "FixedGridViewLayout.h"
#include "FloatMath.h"
#include "ViewProxy.h"

using namespace std;
using namespace PJ;

FixedGridViewLayout::FixedGridViewLayout(Config config) :
    config(config) {}

Vector2
FixedGridViewLayout::ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) {
    GUARDR(proposal.width && proposal.height, Vector2::max)

    // TODO: what if we want the ideal size to be to make them squares? Or rectangles?
    return { *proposal.width, *proposal.height };
}

void FixedGridViewLayout::CalculateFrames(
    Vector2 boundsSize, VectorList<ViewProxy> const& children
) {
    GUARD(owner);
    GUARD(children.size() > 0);
    GUARD(calculatedFrames.size() == 0)

    ViewSizeProposal proposal{ .width = boundsSize.x, .height = boundsSize.y };

    int column{}, row{};

    auto gridSize = config.gridSize;
    auto spacing = config.spacing;

    float cellWidth = (boundsSize.x - (gridSize.x - 1) * config.spacing.x) / children.size();
    float cellHeight = (boundsSize.y - (gridSize.y - 1) * config.spacing.y) / children.size();

    for (auto& child : children) {
        Vec2I loc(column, row);
        float cellX = column * (cellWidth + spacing.x);
        float cellY = row * (cellHeight + spacing.y);

        auto childSize = child.ViewSize(proposal);
        Rect frame{ .origin = { cellX, cellY }, .size = { childSize.x, childSize.y } };

        if (config.xAlignFunc) {
            frame.origin.x = cellX + config.xAlignFunc(cellWidth, frame.size.x);
        }
        if (config.yAlignFunc) {
            frame.origin.y = cellY + config.yAlignFunc(cellHeight, frame.size.y);
        }

        calculatedFrames.push_back(frame);

        column++;
        if (column >= config.gridSize.x) {
            column = 0;
            row++;
        }
    }
}
