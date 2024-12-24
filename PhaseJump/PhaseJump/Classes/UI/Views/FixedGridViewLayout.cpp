#include "FixedGridViewLayout.h"
#include "FloatMath.h"
#include "ViewProxy.h"

using namespace std;
using namespace PJ;

FixedGridViewLayout::FixedGridViewLayout(Config config) :
    gridSize(config.gridSize),
    cellSize(config.cellSize),
    alignment(config.alignment),
    spacing(config.spacing) {}

Vector2
FixedGridViewLayout::ViewSize(ViewSizeProposal proposal, VectorList<ViewProxy> const& children) {
    GUARDR(gridSize.x > 0 && gridSize.y > 0, {})
    GUARDR(cellSize.x > 0 && cellSize.y > 0, {})

    float width = cellSize.x * gridSize.x + (gridSize.x - 1) * spacing.x;
    float height = cellSize.y * gridSize.y + (gridSize.y - 1) * spacing.y;

    return { width, height };
}

void FixedGridViewLayout::CalculateFrames(
    Vector2 boundsSize, VectorList<ViewProxy> const& children
) {
    GUARD(owner);
    GUARD(children.size() > 0);
    calculatedFrames.clear();

    int column{}, row{};

    float cellWidth = cellSize.x;
    float cellHeight = cellSize.y;
    ViewSizeProposal proposal{ .width = cellWidth, .height = cellHeight };

    for (auto& child : children) {
        float cellX = column * (cellWidth + spacing.x);
        float cellY = row * (cellHeight + spacing.y);

        auto childSize = child.ViewSize(proposal);
        Rect frame{ .origin = { cellX, cellY }, .size = { childSize.x, childSize.y } };

        if (alignment.xAlignFunc) {
            frame.origin.x = cellX + alignment.xAlignFunc(cellWidth, frame.size.x);
        }
        if (alignment.yAlignFunc) {
            frame.origin.y = cellY + alignment.yAlignFunc(cellHeight, frame.size.y);
        }

        calculatedFrames.push_back(frame);

        column++;
        if (column >= gridSize.x) {
            column = 0;
            row++;
        }
    }
}
