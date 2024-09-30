#include "MatrixBoard.h"

using namespace std;
using namespace PJ;

template <>
VectorList<MapDirection> PJ::AllCases<MapDirection>() {
    return { MapDirection::Northwest, MapDirection::North, MapDirection::Northeast,
             MapDirection::East,      MapDirection::South, MapDirection::Southeast,
             MapDirection::Southwest, MapDirection::West };
}

MapDirection PJ::Opposite(MapDirection direction) {
    switch (direction) {
    case MapDirection::East:
        return MapDirection::West;
    case MapDirection::West:
        return MapDirection::East;
    case MapDirection::Northwest:
        return MapDirection::Southeast;
    case MapDirection::North:
        return MapDirection::South;
    case MapDirection::Northeast:
        return MapDirection::Southwest;
    case MapDirection::Southeast:
        return MapDirection::Northwest;
    case MapDirection::South:
        return MapDirection::North;
    case MapDirection::Southwest:
        return MapDirection::Northeast;
    }

    return MapDirection::North;
}

/// @return Returns offset in matrix space (top-left is 0, 0)
Vector2Int PJ::MapOffset(MapDirection state) {
    switch (state) {
    case MapDirection::Northwest:
        return Vector2Int(-1, -1);
    case MapDirection::North:
        return Vector2Int(0, -1);
    case MapDirection::Northeast:
        return Vector2Int(1, -1);
    case MapDirection::East:
        return Vector2Int(1, 0);
    case MapDirection::Southeast:
        return Vector2Int(1, 1);
    case MapDirection::South:
        return Vector2Int(0, 1);
    case MapDirection::Southwest:
        return Vector2Int(-1, 1);
    case MapDirection::West:
        return Vector2Int(-1, 0);
    }

    return Vector2Int(0, 0);
}

MatrixBoard::MatrixBoard(Vector2Int size) :
    matrix(size) {}

MatrixBoard::MatrixType& MatrixBoard::Matrix() {
    return matrix;
}

bool MatrixBoard::IsCellOccupied(Vector2Int origin) {
    try {
        auto& cell = matrix.CellAt(origin);
        return cell.piece != nullptr;
    } catch (...) {
        return true;
    }
}

VectorList<Vector2Int> MatrixBoard::PieceLocationsAt(Vector2Int origin, MatrixPiece const& piece) {
    VectorList<Vector2Int> result;

    for (int x = 0; x < piece.Width(); x++) {
        for (int y = 0; y < piece.Height(); y++) {
            auto pieceLoc = Vector2Int(x, y);

            try {
                // Skip empty cells in the piece
                auto isOccupiedByPiece = piece.Matrix().CellAt(pieceLoc);
                if (!isOccupiedByPiece) {
                    continue;
                }
            } catch (...) {}

            auto location = origin + pieceLoc;
            GUARDR(matrix.IsValidLocation(location), {})
            result.push_back(location);
        }
    }

    return result;
}

bool MatrixBoard::Put(SP<MatrixPiece> piece, Vector2Int origin) {
    GUARDR(piece, false)
    GUARDR(!IsPieceBlockedAt(origin, *piece), false)
    GUARDR(piece->board == nullptr, false)

    piece->board = this;
    piece->origin = origin;
    pieces.insert(piece);

    for (int x = 0; x < piece->Width(); x++) {
        for (int y = 0; y < piece->Height(); y++) {
            Vector2Int pieceLoc(x, y);

            // Skip empty cells in the piece
            auto isOccupiedByPiece = piece->Matrix().CellAt(pieceLoc);
            GUARD_CONTINUE(isOccupiedByPiece);

            try {
                auto& cell = matrix.CellAt(origin + pieceLoc);
                cell.piece = piece;
                cell.pieceLoc = pieceLoc;
            } catch (...) {}
        }
    }

    return true;
}

SP<MatrixPiece> MatrixBoard::PieceAt(Vector2Int location) {
    try {
        auto& cell = matrix.CellAt(location);
        return cell.piece;
    } catch (...) {
        return nullptr;
    }
}

SP<MatrixPiece> MatrixBoard::PieceInDirection(Vector2Int location, MapDirection direction) {
    Vector2Int offset = MapOffset(direction);
    auto checkLocation = location + offset;
    return PieceAt(checkLocation);
}

void MatrixBoard::RemovePieceAt(Vector2Int location) {
    Remove(PieceAt(location));
}

void MatrixBoard::Remove(SP<MatrixPiece> piece) {
    GUARD(piece)
    GUARD_LOG(piece->board == this, "ERROR. Can't remove piece from a different board")
    piece->board = nullptr;

    auto pieceLocations = PieceLocationsAt(piece->origin, *piece);

    for (auto& location : pieceLocations) {
        try {
            auto& cell = matrix.CellAt(location);
            GUARD_CONTINUE(cell.piece == piece)
            cell.piece = nullptr;
        } catch (...) {}
    }

    pieces.erase(piece);
}

bool MatrixBoard::IsPieceBlockedAt(Vector2Int origin, MatrixPiece const& piece) {
    return IsPieceBlockedAtExclude(origin, piece, {});
}

bool MatrixBoard::IsPieceBlockedAtExclude(
    Vector2Int origin, MatrixPiece const& piece, UnorderedSet<SP<MatrixPiece>> excludePieces
) {
    for (int x = 0; x < piece.Width(); x++) {
        for (int y = 0; y < piece.Height(); y++) {
            Vector2Int pieceLoc(x, y);

            // Skip empty cells in the piece
            auto isOccupiedByPiece = piece.Matrix().CellAt(pieceLoc);
            if (!isOccupiedByPiece) {
                continue;
            }

            auto location = origin + pieceLoc;

            if (IsCellOccupied(location)) {
                // If specified, ignore some pieces for check
                if (excludePieces.size() > 0) {
                    try {
                        auto& cell = matrix.CellAt(location);
                        if (excludePieces.contains(cell.piece)) {
                            continue;
                        }
                    } catch (...) {}
                }

                return true;
            }
        }
    }

    return false;
}
