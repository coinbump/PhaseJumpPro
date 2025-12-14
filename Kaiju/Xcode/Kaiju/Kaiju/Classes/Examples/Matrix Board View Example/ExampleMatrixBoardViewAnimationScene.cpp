#include "ExampleMatrixBoardViewAnimationScene.h"

using namespace std;
using namespace PJ;
using namespace Example;

void MatrixBoardViewAnimation::Scene::LoadInto(WorldNode& root) {
    auto worldSize = this->worldSize;
    auto matrixSize = this->matrixSize;
    auto moveInDirections = this->moveInDirections;
    auto count = std::min(matrixSize.x * matrixSize.y, this->count);

    QB(root)
        .OrthoStandard(QuickBuilder::OrthoStandardConfig{ .clearColor = Color32(31, 36, 84) })
        .And("Matrix view")
        .ScaleWithWindow(worldSize, 0.8f)
        .With<MatrixBoardView>(worldSize, matrixSize)
        .ModifyLatest<MatrixBoardView>([this](auto& boardView) { this->boardView = &boardView; })
        .Grid(worldSize, matrixSize, Color::red, 2)
        .CycleHueEffect("", SwitchState::On, 200)
        .Repeat(count, [=, this](QuickBuilder& qb) {
            VectorList<String> shapes{ "*", "**&**" };

            StandardRandom random;
            auto shapeChoice = random.Choice((int)shapes.size());
            auto shape = shapes[shapeChoice];
            auto circleRadius = (shapeChoice + 1) * 10;

            qb.And("Matrix piece")
                .With<MatrixPieceHandler>(MatrixPieceHandler::Config{ .shapeString = shape })
                .Circle(random.VaryFloat(circleRadius, circleRadius * 0.2f), Color::yellow)
                .ModifyLatest<MatrixPieceHandler>([=, this](auto& handler) {
                    StandardRandom random;

                    // Search for an empty space to put the new piece
                    while (!boardView->Put(handler, handler.startOrigin)) {
                        int x = random.IntValue(0, matrixSize.x - 1);
                        int y = random.IntValue(0, matrixSize.y - 1);
                        handler.startOrigin = { x, y };
                    }

                    // Vary the move piece animate duration
                    float animateDuration = random.VaryFloat(0.3f, 0.1f);

                    // Rate limiter allows a piece move every N seconds
                    auto limiter = MAKE<RateLimiter>(animateDuration + 0.2f);
                    MatrixPieceHandler* handlerPtr = &handler;

                    RateLimiter::OnFireFunc overrideFunc = [=, this](auto& limiter) {
                        GUARD(boardView)
                        auto positionMaker =
                            AnimateFuncs::PositionMaker(animateDuration, EaseFuncs::inBack);

                        if (moveInDirections) {
                            // Move in map directions
                            auto allCases = AllCases<MapDirection>();
                            auto choice = StandardRandom().IntValue(0, (int)allCases.size() - 1);
                            auto direction = allCases[choice];
                            boardView->Move(handlerPtr->piece, direction, positionMaker);
                        } else {
                            // Move to a random new location
                            auto x = StandardRandom().IntValue(0, boardView->board.Size().x - 1);
                            auto y = StandardRandom().IntValue(0, boardView->board.Size().y - 1);
                            boardView->Move(handlerPtr->piece, Vector2Int{ x, y }, positionMaker);
                        }
                    };
                    Override(limiter->onFireFunc, overrideFunc);

                    handler.GetUpdatables().AddContinue([=](auto time) {
                        limiter->OnUpdate(time);
                        limiter->Fire();
                    });
                })
                .Pop();
        });
}
