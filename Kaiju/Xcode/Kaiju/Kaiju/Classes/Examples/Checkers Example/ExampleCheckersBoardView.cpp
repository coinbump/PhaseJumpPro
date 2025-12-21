#include "ExampleCheckersBoardView.h"
#include "ExampleCheckersPiecesView.h"

using namespace std;
using namespace PJ;
using namespace Example;
using namespace Checkers;

void BoardView::Awake() {
    Base::Awake();

    Build();
    PutChildPieces();
}

void BoardView::Build() {
    GUARD(owner)

    QB qb(*owner);

    auto piecesView = this->piecesView;
    GUARD(piecesView)

    auto matrixSize = board.Size();

    for (int y = 0; y < matrixSize.y; y++) {
        for (int x = 0; x < matrixSize.x; x++) {
            GUARD_CONTINUE(piecesView->IsDarkSquareAt({ x, y }))

            Vec2I loc(x, y);

            qb.And("Drop node")
                .With<MatrixPieceHandler>(MatrixPieceHandler::Config{ .origin = loc,
                                                                      .shapeString = "*" })
                .With<DropTarget>()
                .SquareCollider(CellSize().x)
                .SquareFrame(CellSize().x, Color::white, 2)
                .ModifyLatest<DropTarget>([=](DropTarget& target) {
                    Renderer* renderer = target.owner->TypeComponent<Renderer>();
                    GUARD(renderer)

                    target.canAcceptDragFunc = [=](DropTarget& target, auto& dragModel) {
                        return !piecesView->board.IsCellOccupied(loc);
                    };

                    target.onStateChangeFunc = [=](DropTarget& target) {
                        renderer->Enable(target.State() == DropTarget::StateType::DragAccept);
                    };

                    target.onDropFunc = [=](DropTarget& target, DragModel const& dragModel) {
                        GUARD(dragModel.DragHandler())

                        auto pieceHandler =
                            dragModel.DragHandler()->owner->TypeComponent<MatrixPieceHandler>();
                        GUARD(pieceHandler && pieceHandler->piece)

                        piecesView->TryMove(*pieceHandler, loc);
                    };
                })
                .Pop();
        }
    }
}
