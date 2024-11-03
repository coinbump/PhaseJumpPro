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

            Vec2i loc(x, y);

            qb.And("Drop node")
                .With<MatrixPieceHandler>(loc, "*")
                .With<SomeDropTarget>()
                .SquareCollider(CellSize().x)
                .SquareFrame(CellSize().x, Color::white, 2)
                .ModifyLatest<SomeDropTarget>([=](SomeDropTarget& target) {
                    SP<SomeRenderer> renderer = target.owner->TypeComponent<SomeRenderer>();
                    GUARD(renderer)

                    target.canAcceptDragFunc = [=](SomeDropTarget& target, auto& dragModel) {
                        return !piecesView->board.IsCellOccupied(loc);
                    };

                    target.onStateChangeFunc = [=](SomeDropTarget& target) {
                        renderer->Enable(target.State() == SomeDropTarget::StateType::DragAccept);
                    };

                    target.onDropFunc = [=](SomeDropTarget& target, DragModel const& dragModel) {
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
