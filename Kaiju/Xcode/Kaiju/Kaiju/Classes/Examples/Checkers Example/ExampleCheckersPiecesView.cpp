#include "ExampleCheckersPiecesView.h"

using namespace std;
using namespace PJ;
using namespace Example;
using namespace Checkers;

bool PiecesView::IsDarkSquareAt(Vec2I location) {
    auto isYEven = location.y % 2 == 0;
    auto isXEven = location.x % 2 == 0;
    return isYEven ? !isXEven : isXEven;
}

void PiecesView::Awake() {
    Base::Awake();

    Build();
    PutChildPieces();
}

void PiecesView::Build() {
    GUARD(owner)

    QB qb(*owner);

    float pieceRadius = CellSize().x * 0.4f;
    auto matrixSize = board.Size();

    // Populate black pieces
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < matrixSize.x; x++) {
            Vector2Int loc(x, y);
            GUARD_CONTINUE(IsDarkSquareAt(loc))

            qb.And("Black piece")
                .Circle(pieceRadius, Color::black)
                .With<MatrixPieceHandler>(loc, "*")
                .ModifyLatest<MatrixPieceHandler>([](auto& handler) {
                    handler.core.typeTags.insert("black");
                })
                .SetValveDurations(0.45f, 0.45f)
                .HoverScaleToPingPong(1.15f)
                .CircleCollider(pieceRadius)
                .DragSnapBack()
                .SetAnimateDelay(0)
                .SetAnimateEase(EaseFuncs::linear)
                .OpacityIn()
                .SetAnimateDelay(StandardRandom().Value() * 0.15f)
                .SetAnimateEase(EaseFuncs::outCubed)
                .ScaleIn(3)
                .Pop();
        }
    }

    // Populate red pieces
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < matrixSize.x; x++) {
            Vec2I loc(x, matrixSize.y - 1 - y);
            GUARD_CONTINUE(IsDarkSquareAt(loc))

            qb.And("Red piece")
                .Circle(pieceRadius, Color::red)
                .With<MatrixPieceHandler>(loc, "*")
                .ModifyLatest<MatrixPieceHandler>([](auto& handler) {
                    handler.core.typeTags.insert("red");
                })
                .SetValveDurations(0.45f, 0.45f)
                .HoverScaleToPingPong(1.15f)
                .CircleCollider(pieceRadius)
                .DragSnapBack()
                .SetAnimateDelay(0)
                .SetAnimateEase(EaseFuncs::linear)
                .OpacityIn()
                .SetAnimateDelay(StandardRandom().Value() * 0.15f)
                .SetAnimateEase(EaseFuncs::outCubed)
                .ScaleIn(3)
                .Pop();
        }
    }
}

void PiecesView::TryMove(MatrixPieceHandler& pieceHandler, Vec2I location) {
    GUARD(pieceHandler.piece)
    auto pieceLocation = pieceHandler.piece->Origin();

    GUARD(CanMove(pieceHandler, location))

    Move(pieceHandler.piece, location, {});

    // If we jumped a piece, remove the jumped piece
    if (std::abs(location.y - pieceLocation.y) == 2 &&
        std::abs(location.x - pieceLocation.x) == 2) {
        Vec2I jumpedLocation(
            pieceLocation.x + (location.x - pieceLocation.x) / 2,
            pieceLocation.y + (location.y - pieceLocation.y) / 2
        );

        try {
            auto& jumpedPieceNode = NodeAt(jumpedLocation);
            RemoveAt(jumpedLocation, DestroyType::Keep);

            jumpedPieceNode.RemoveType<DragHandler2D>();
            jumpedPieceNode.RemoveType<SomeCollider>();

            QB(jumpedPieceNode)
                .SetAnimateDuration(0.2f)
                .SetAnimateEase(EaseFuncs::inCubed)
                .OpacityTo(0)
                .ScaleTo(0.1)
                .DestroyAfterAnimate();
        } catch (...) {}
    }
}

bool PiecesView::CanMove(MatrixPieceHandler& pieceHandler, Vec2I location) {
    GUARDR(pieceHandler.piece, false)
    GUARDR(!board.IsCellOccupied(location), false)

    auto pieceLocation = pieceHandler.piece->Origin();
    auto isBlack = pieceHandler.core.typeTags.contains("black");

    if (isBlack) {
        // Ok to move 1 diagonal.
        if (location.y - pieceLocation.y == 1) {
            return std::abs(location.x - pieceLocation.x) == 1;
        }

        // In this example, no double jumps are allowed.
        if (location.y - pieceLocation.y == 2 && std::abs(location.x - pieceLocation.x) == 2) {
            Vec2I jumpedLocation(
                pieceLocation.x + (location.x - pieceLocation.x) / 2, pieceLocation.y + 1
            );

            try {
                auto& jumpedPieceNode = NodeAt(jumpedLocation);
                auto jumpedPieceComponent = jumpedPieceNode.TypeComponent<MatrixPieceHandler>();
                if (jumpedPieceComponent &&
                    jumpedPieceComponent->core.typeTags.contains("black") != isBlack) {
                    return true;
                }
            } catch (...) {}
        }
    } else {
        if (location.y - pieceLocation.y == -1) {
            return std::abs(location.x - pieceLocation.x) == 1;
        }

        // In this example, no double jumps are allowed.
        if (location.y - pieceLocation.y == -2 && std::abs(location.x - pieceLocation.x) == 2) {
            Vec2I jumpedLocation(
                pieceLocation.x + (location.x - pieceLocation.x) / 2, pieceLocation.y - 1
            );

            try {
                auto& jumpedPieceNode = NodeAt(jumpedLocation);
                auto jumpedPieceComponent = jumpedPieceNode.TypeComponent<MatrixPieceHandler>();
                if (jumpedPieceComponent &&
                    jumpedPieceComponent->core.typeTags.contains("black") != isBlack) {
                    return true;
                }
            } catch (...) {}
        }
    }

    return false;
}
