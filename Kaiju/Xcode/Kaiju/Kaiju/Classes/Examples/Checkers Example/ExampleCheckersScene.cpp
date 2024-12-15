#include "ExampleCheckersScene.h"
#include "ExampleCheckersBoardView.h"
#include "ExampleCheckersPiecesView.h"

using namespace std;
using namespace PJ;
using namespace Example;

void Checkers::Scene::LoadInto(WorldNode& root) {
    auto worldSize = this->worldSize;

    auto piecesView = MAKE<PiecesView>(worldSize);

    QB(root)
        .Named("Checkers scene")
        .OrthoStandard(Color::gray)
        .And("Animator")
        .SetAnimateDuration(1)
        .SetAnimateEase(EaseFuncs::outElastic)
        .RotateIn(Angle::WithDegrees(-45))
        .MoveIn(Vector3(-300, 0, 0))
        .ScaleIn(0.7f)
        .And("Board view")
        .With<BoardView>(worldSize, piecesView.get())
        .With<MatrixRenderer>(worldSize)
        .Pop()
        .And("Piece view")
        .With(piecesView)
        .ModifyLatest<PiecesView>([this](auto& boardView) { this->boardView = &boardView; });
}
