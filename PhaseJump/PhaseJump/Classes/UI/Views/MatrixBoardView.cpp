#include "MatrixBoardView.h"

using namespace std;
using namespace PJ;

MatrixBoardView::MatrixBoardView(Vector2 worldSize, Vector2Int matrixSize) :
    board(matrixSize) {
    SetFrameSize(worldSize);

    signalHandlers[SignalId::AddChildNode] = [](auto& owner, auto& signal) {
        auto& event = *(static_cast<AddChildNodeEvent const*>(&signal));

        auto handler = event.node->TypeComponent<MatrixPieceHandler>();
        if (handler) {
            static_cast<This*>(&owner)->Put(*handler, handler->startOrigin);
        }
    };
}

/// Remove the piece at the specified location and optionally destroy the corresponding world node
void MatrixBoardView::RemoveAt(Vector2Int location, DestroyType destroyNode) {
    if (destroyNode == DestroyType::Destroy) {
        try {
            auto& node = NodeAt(location);
            node.Destroy();
        } catch (...) {}
    }
    board.RemovePieceAt(location);
}

/// Remove all pieces and optionally destroy the corresponding world node
void MatrixBoardView::RemoveAll(DestroyType destroyNode) {
    auto pieces = board.Pieces();
    for (auto& piece : pieces) {
        RemoveAt(piece->Origin(), destroyNode);
    }
}

/// Put the piece handler's piece into the board and move the piece's node to the cell position
bool MatrixBoardView::Put(MatrixPieceHandler& handler, Vector2Int origin) {
    GUARDR(handler.owner && handler.piece, false)
    GUARDR_LOG(
        board.Put(handler.piece, origin), false,
        "Error. Can't place Matrix node at " + origin.ToString()
    )

    auto nodePosition = NodeLocalPosition(handler);
    handler.owner->SetLocalPosition(nodePosition);
    return true;
}

/// @return Returns the cell size
Vector2 MatrixBoardView::CellSize() const {
    GUARDR(board.IsValid(), vec2Zero)

    auto worldSize = WorldSize();
    GUARDR(worldSize, vec2Zero);

    auto worldSize2 = (Vector2)(*worldSize);
    return worldSize2 / (Vector2(board.Size().x, board.Size().y));
}

/// Puts this piece on the board, deleting any existing pieces that are blocking
/// Optionally destroy the corresponding nodes
bool MatrixBoardView::Replace(
    MatrixPieceHandler& handler, Vector2Int origin, DestroyType destroyNode
) {
    GUARDR(handler.owner && handler.piece, false)

    auto pieceLocations = board.PieceLocationsAt(origin, *handler.piece);
    GUARDR(!IsEmpty(pieceLocations), false)

    for (auto& location : pieceLocations) {
        RemoveAt(location, destroyNode);
    }

    return Put(handler, origin);
}

/// Converts a position in view reading coordinates to a matrix location
Vector2Int MatrixBoardView::ViewPositionToLocation(Vector2 viewPosition) {
    auto cellSize = CellSize();

    // Avoid divide by zero
    GUARDR(cellSize.x > 0 && cellSize.y > 0, Vector2Int(0, 0))

    auto column = (int)(viewPosition.x / cellSize.x);
    auto row = (int)(viewPosition.y / cellSize.y);

    return Vector2Int(column, row);
}

/// @return Returns the node for the cell location, or throws an exception if the location is
/// invalid
WorldNode& MatrixBoardView::NodeAt(Vector2Int loc) {
    auto piece = board.PieceAt(loc);
    GUARD_THROW(piece && piece->owner, std::out_of_range("No node at loc"))

    MatrixPieceHandler* handler = static_cast<MatrixPieceHandler*>(piece->owner);
    GUARD_THROW(handler->owner, std::out_of_range("No node at loc"))

    return *handler->owner;
}

Vector3 MatrixBoardView::NodeLocalPosition(MatrixPieceHandler const& handler) {
    GUARDR(handler.piece, Vector3::zero)

    auto pieceOrigin = handler.piece->Origin();
    auto pieceSize = handler.piece->Size();

    auto topLeftCellLocalPosition = LocationToLocalPosition(pieceOrigin);
    auto bottomRightCellLocalPosition =
        LocationToLocalPosition(pieceOrigin + Vector2Int(pieceSize.x - 1, pieceSize.y - 1));

    auto x = topLeftCellLocalPosition.x +
             ((bottomRightCellLocalPosition.x - topLeftCellLocalPosition.x) / 2.0f) * vecRight;
    auto y =
        topLeftCellLocalPosition.y +
        std::abs((bottomRightCellLocalPosition.y - topLeftCellLocalPosition.y) / 2.0f) * vecDown;

    return Vector2(x, y);
}

Vector3 MatrixBoardView::LocationToLocalPosition(Vector2Int loc) {
    auto x =
        (WorldSize()->x / 2.0f) * vecLeft + (CellSize().x / 2.0f + loc.x * CellSize().x) * vecRight;
    auto y =
        (WorldSize()->y / 2.0f) * vecUp + (CellSize().y / 2.0f + loc.y * CellSize().y) * vecDown;

    return Vector3(x, y, 0);
}

std::optional<Vector2Int> MatrixBoardView::LocalPositionToLocation(Vector2 localPosition) {
    return WorldPositionToLocation(owner->LocalToWorld(localPosition));
}

std::optional<Vector2Int> MatrixBoardView::WorldPositionToLocation(Vector2 worldPosition) {
    auto topLeft = TopLeftWorldPosition();

    Vector2 viewPosition(worldPosition.x - topLeft.x, std::abs(worldPosition.y - topLeft.y));
    GUARDR(IsViewPositionInside(viewPosition), std::nullopt)

    auto cell = ViewPositionToLocation(viewPosition);
    return cell;
}

/// Move the piece from its current location to a new location in the specified direction
MatrixBoardView::MoveResult MatrixBoardView::Move(
    SP<MatrixPiece> piece, MapDirection direction, MakeAnimatorFunc makeAnimatorFunc
) {
    GUARDR(piece, MoveResult::Fail)

    auto newOrigin = piece->Origin() + MapOffset(direction);
    return Move(piece, newOrigin, makeAnimatorFunc);
}

Vector3 MatrixBoardView::LocationToWorldPosition(Vector2Int cell) {
    auto topLeft = TopLeftWorldPosition();
    auto x = topLeft.x + (CellSize().x / 2.0f + cell.x * CellSize().x) * vecRight;
    auto y = topLeft.y + (CellSize().y / 2.0f + cell.y * CellSize().y) * vecDown;

    return Vector3(x, y, topLeft.z);
}

MatrixBoardView::MoveResult MatrixBoardView::Move(
    SP<MatrixPiece> piece, Vector2Int newOrigin, MakeAnimatorFunc makeAnimatorFunc
) {
    GUARDR(piece, MoveResult::Fail)
    GUARDR_LOG(piece->board == &board, MoveResult::Fail, "ERROR. Piece doesn't belong to board")
    GUARDR_LOG(piece->owner, MoveResult::Fail, "ERROR. Piece owner is missing")

    auto& handler = *(static_cast<MatrixPieceHandler*>(piece->owner));
    GUARDR(!handler.IsAnimating(), MoveResult::Animating)

    auto& node = *handler.owner;
    auto oldOrigin = piece->Origin();
    GUARDR(newOrigin != oldOrigin, MoveResult::Fail)

    UnorderedSet<SP<MatrixPiece>> excludeList;
    excludeList.insert(piece);
    if (board.IsPieceBlockedAtExclude(newOrigin, *piece, excludeList)) {
        return MoveResult::Blocked;
    }

    board.Remove(piece);
    board.Put(piece, newOrigin);

    Vector3 endPosition = NodeLocalPosition(handler);

    if (makeAnimatorFunc) {
        auto animator = makeAnimatorFunc(node.transform.LocalPosition(), endPosition, node);
        handler.SetAnimator(animator);
    } else {
        node.SetLocalPosition(endPosition);
    }

    return MoveResult::Success;
}
