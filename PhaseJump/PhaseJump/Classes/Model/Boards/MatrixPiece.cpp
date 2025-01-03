#include "MatrixPiece.h"

using namespace std;
using namespace PJ;

bool MatrixPiece::BuildFromShape(VectorList<String> const& pieceShape) {
    int height = (int)pieceShape.size();
    int width = 0;

    for (auto& pieceString : pieceShape) {
        width = std::max(width, (int)pieceString.size());
    }

    if (height <= 0 || width <= 0) {
        PJ::Log("Error. Invalid piece shape");
        return false;
    }

    matrix.ResizeFast(Vector2Int(width, height));
    Clear();

    auto y = 0;
    for (auto& pieceString : pieceShape) {
        for (int x = 0; x < width && x < pieceString.size(); x++) {
            auto character = pieceString[x];
            /// Space or `_` can be used to designate empty cells
            if (character != ' ' && character != '_') {
                matrix.SetCell(Vector2Int(x, y), true);
            }
        }

        y++;
    }

    return true;
}
