#include "gtest/gtest.h"
#include "TypeClass.h"
#include "MatrixBoardView.h"
#include "World.h"
#include "Macros.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace MatrixBoardViewTests {
    using DestroyType = PJ::MatrixBoardView::DestroyType;
    using MoveResult = PJ::MatrixBoardView::MoveResult;
}

using namespace MatrixBoardViewTests;

TEST(MatrixBoardView, Init) {
    MatrixBoardView sut({}, {8, 10});
    EXPECT_EQ(sut.BoardSize(), Vec2I(8, 10));
}

TEST(MatrixBoardView, PieceHandler) {
    VectorList<String> shape{"**", " **"};
    MatrixPieceHandler sut({3, 4}, shape);

    ASSERT_NE(nullptr, sut.piece);
    EXPECT_EQ(Vec2I(3, 2), sut.Size());
    EXPECT_EQ(Vec2I(3, 4), sut.startOrigin);
}

TEST(MatrixBoardView, Put) {
    World world;

    VectorList<String> shape{"**", "* "};
    MatrixBoardView sut({}, {3, 3});

    {
        auto node = MAKE<WorldNode>();
        world.Add(node);
        auto handler = MAKE<MatrixPieceHandler>(Vec2I{0, 0}, shape);
        node->Add(handler);

        ASSERT_NE(nullptr, handler->piece);
        EXPECT_EQ(Vec2I(2, 2), handler->Size());

        EXPECT_TRUE(sut.Put(*handler, Vec2I(0, 0)));
    }
    {
        auto node = MAKE<WorldNode>();
        world.Add(node);
        auto handler = MAKE<MatrixPieceHandler>(Vec2I{0, 0}, shape);
        node->Add(handler);

        ASSERT_NE(nullptr, handler->piece);
        EXPECT_EQ(Vec2I(2, 2), handler->Size());

        EXPECT_TRUE(sut.Put(*handler, Vec2I(1, 1)));
    }
    {
        auto node = MAKE<WorldNode>();
        world.Add(node);
        auto handler = MAKE<MatrixPieceHandler>(Vec2I{0, 0}, shape);
        node->Add(handler);

        ASSERT_NE(nullptr, handler->piece);
        EXPECT_EQ(Vec2I(2, 2), handler->Size());

        EXPECT_FALSE(sut.Put(*handler, Vec2I(2, 2)));
        EXPECT_FALSE(sut.Put(*handler, Vec2I(1, 0)));
    }
}

TEST(MatrixBoardView, CellSize) {
    MatrixBoardView sut({100, 50}, {2, 2});
    EXPECT_EQ(Vector2(50, 25), sut.CellSize());
}

TEST(MatrixBoardView, NodeAt) {
    World world;

    VectorList<String> shape{"**", "* "};
    MatrixBoardView sut({100, 100}, {2, 2});

    auto node = MAKE<WorldNode>();
    world.Add(node);
    auto handler = MAKE<MatrixPieceHandler>(Vec2I{3, 4}, shape);
    node->Add(handler);
    EXPECT_TRUE(sut.Put(*handler, Vec2I(0, 0)));

    EXPECT_NO_THROW(sut.NodeAt(Vec2I(0, 0)));
    EXPECT_NO_THROW(sut.NodeAt(Vec2I(1, 0)));
    EXPECT_NO_THROW(sut.NodeAt(Vec2I(0, 1)));

    EXPECT_EQ(node.get(), &sut.NodeAt(Vec2I(0, 0)));
    EXPECT_EQ(node.get(), &sut.NodeAt(Vec2I(1, 0)));
    EXPECT_EQ(node.get(), &sut.NodeAt(Vec2I(0, 1)));

    EXPECT_ANY_THROW(sut.NodeAt(Vec2I(1, 1)));
}

TEST(MatrixBoardView, ViewPositionToLocation) {
    VectorList<String> shape{"**", "* "};
    MatrixBoardView sut({100, 100}, {2, 2});

    EXPECT_EQ(Vec2I(0, 0), sut.ViewPositionToLocation(Vector2(0, 0)));
    EXPECT_EQ(Vec2I(0, 0), sut.ViewPositionToLocation(Vector2(25, 25)));
    EXPECT_EQ(Vec2I(1, 0), sut.ViewPositionToLocation(Vector2(75, 25)));
    EXPECT_EQ(Vec2I(1, 1), sut.ViewPositionToLocation(Vector2(75, 75)));
}

TEST(MatrixBoardView, LocationToLocalPosition) {
    World world;
    auto node = MAKE<WorldNode>();
    world.Add(node);

    auto sut = MAKE<MatrixBoardView>(Vector2{100, 100}, Vec2I{2, 2});
    node->Add(sut);

    EXPECT_EQ(Vector3(25 * vecLeft, 25 * vecUp, 0), sut->LocationToLocalPosition(Vec2I(0, 0)));
    EXPECT_EQ(Vector3(25 * vecRight, 25 * vecDown, 0), sut->LocationToLocalPosition(Vec2I(1, 1)));
}

TEST(MatrixBoardView, LocalPositionToLocation) {
    World world;
    auto node = MAKE<WorldNode>();
    world.Add(node);

    auto sut = MAKE<MatrixBoardView>(Vector2{100, 100}, Vec2I{2, 2});
    node->Add(sut);

    EXPECT_EQ(Vec2I(0, 0), sut->LocalPositionToLocation(Vector3(25 * vecLeft, 25 * vecUp, 0)));
    EXPECT_EQ(Vec2I(1, 1), sut->LocalPositionToLocation(Vector3(25 * vecRight, 25 * vecDown, 0)));
}

TEST(MatrixBoardView, LocationToWorldPosition) {
    World world;
    auto node = MAKE<WorldNode>();
    world.Add(node);

    VectorList<String> shape{"**", "* "};
    auto sut = MAKE<MatrixBoardView>(Vector2{100, 100}, Vec2I{2, 2});

    node->Add(sut);
    node->transform.SetWorldPosition(Vector3{10, 5, 0});

    EXPECT_EQ(Vector3(25 * vecLeft + 10, 25 * vecUp + 5, 0), sut->LocationToWorldPosition(Vec2I(0, 0)));
    EXPECT_EQ(Vector3(25 * vecRight + 10, 25 * vecDown + 5, 0), sut->LocationToWorldPosition(Vec2I(1, 1)));
}

TEST(MatrixBoardView, WorldPositionToLocation) {
    World world;
    auto node = MAKE<WorldNode>();
    world.Add(node);

    VectorList<String> shape{"**", "* "};
    auto sut = MAKE<MatrixBoardView>(Vector2{100, 100}, Vec2I{2, 2});

    node->Add(sut);
    node->transform.SetWorldPosition(Vector3{10, 5, 0});

    EXPECT_EQ(Vec2I(0, 0), sut->WorldPositionToLocation(Vector3(25 * vecLeft + 10, 25 * vecUp + 5, 0)));
    EXPECT_EQ(Vec2I(1, 1), sut->WorldPositionToLocation(Vector3(25 * vecRight + 10, 25 * vecDown + 5, 0)));
}

TEST(MatrixBoardView, NodeLocalPosition) {
    World world;
    auto node = MAKE<WorldNode>();
    world.Add(node);

    auto sut = MAKE<MatrixBoardView>(Vector2{100, 100}, Vec2I{2, 2});
    node->Add(sut);

    VectorList<String> shape{"**", "* "};
    auto handler = MAKE<MatrixPieceHandler>(Vec2I{0, 0}, shape);
    auto childNode = MAKE<WorldNode>();
    node->Add(childNode);
    childNode->Add(handler);

    sut->Put(*handler, handler->startOrigin);

    EXPECT_EQ(Vector3(0, 0, 0), sut->NodeLocalPosition(*handler));
}

TEST(MatrixBoardView, RemoveAtNoDestroy) {
    World world;
    auto node = MAKE<WorldNode>();
    world.Add(node);

    auto sut = MAKE<MatrixBoardView>(Vector2{100, 100}, Vec2I{2, 2});
    node->Add(sut);

    VectorList<String> shape{"**", "* "};
    auto handler = MAKE<MatrixPieceHandler>(Vec2I{0, 0}, shape);
    auto childNode = MAKE<WorldNode>();
    node->Add(childNode);
    childNode->Add(handler);

    sut->Put(*handler, handler->startOrigin);
    EXPECT_EQ(&sut->board, handler->piece->board);
    EXPECT_NO_THROW(sut->NodeAt(Vec2I(0, 0)));

    sut->RemoveAt({0, 0}, DestroyType::Keep);
    EXPECT_FALSE(childNode->IsDestroyed());
    EXPECT_EQ(nullptr, handler->piece->board);

    EXPECT_ANY_THROW(sut->NodeAt(Vec2I(0, 0)));
}

TEST(MatrixBoardView, RemoveAtDestroy) {
    World world;
    auto node = MAKE<WorldNode>();
    world.Add(node);

    auto sut = MAKE<MatrixBoardView>(Vector2{100, 100}, Vec2I{2, 2});
    node->Add(sut);

    VectorList<String> shape{"**", "* "};
    auto handler = MAKE<MatrixPieceHandler>(Vec2I{0, 0}, shape);
    auto childNode = MAKE<WorldNode>();
    node->Add(childNode);
    childNode->Add(handler);

    sut->Put(*handler, handler->startOrigin);
    sut->RemoveAt({0, 0}, DestroyType::Destroy);
    EXPECT_TRUE(childNode->IsDestroyed());
}

TEST(MatrixBoardView, RemoveAll) {
    World world;
    auto parentNode = MAKE<WorldNode>();

    auto sut = MAKE<MatrixBoardView>(Vector2{100, 100}, Vec2I{2, 2});
    parentNode->Add(sut);

    {
        auto node = MAKE<WorldNode>();
        parentNode->Add(node);

        VectorList<String> shape{"*"};
        auto handler = MAKE<MatrixPieceHandler>(Vec2I{0, 0}, shape);
        node->Add(handler);

        sut->Put(*handler, handler->startOrigin);
        EXPECT_EQ(&sut->board, handler->piece->board);
        EXPECT_NO_THROW(sut->NodeAt(Vec2I(0, 0)));
    }
    {
        auto node = MAKE<WorldNode>();
        parentNode->Add(node);

        VectorList<String> shape{"*"};
        auto handler = MAKE<MatrixPieceHandler>(Vec2I{1, 1}, shape);
        node->Add(handler);

        sut->Put(*handler, handler->startOrigin);
        EXPECT_EQ(&sut->board, handler->piece->board);
        EXPECT_NO_THROW(sut->NodeAt(Vec2I(1, 1)));
    }

    sut->RemoveAll();
    EXPECT_ANY_THROW(sut->NodeAt(Vec2I(0, 0)));
    EXPECT_ANY_THROW(sut->NodeAt(Vec2I(1, 1)));
}

TEST(MatrixBoardView, Replace) {
    World world;
    auto parentNode = MAKE<WorldNode>();

    auto sut = MAKE<MatrixBoardView>(Vector2{100, 100}, Vec2I{2, 2});
    parentNode->Add(sut);

    {
        auto node = MAKE<WorldNode>();
        parentNode->Add(node);

        VectorList<String> shape{"**"};
        auto handler = MAKE<MatrixPieceHandler>(Vec2I{0, 0}, shape);
        node->Add(handler);

        sut->Put(*handler, handler->startOrigin);
        EXPECT_EQ(&sut->board, handler->piece->board);
        EXPECT_NO_THROW(sut->NodeAt(Vec2I(0, 0)));
        EXPECT_NO_THROW(sut->NodeAt(Vec2I(1, 0)));
        EXPECT_EQ(handler->owner, &sut->NodeAt(Vec2I(0, 0)));
        EXPECT_EQ(handler->owner, &sut->NodeAt(Vec2I(1, 0)));
    }
    {
        auto node = MAKE<WorldNode>();
        parentNode->Add(node);

        VectorList<String> shape{"*"};
        auto handler = MAKE<MatrixPieceHandler>(Vec2I{0, 0}, shape);
        node->Add(handler);

        sut->Replace(*handler, handler->startOrigin);
        EXPECT_EQ(&sut->board, handler->piece->board);
        EXPECT_NO_THROW(sut->NodeAt(Vec2I(0, 0)));
        EXPECT_ANY_THROW(sut->NodeAt(Vec2I(1, 0)));
        EXPECT_EQ(handler->owner, &sut->NodeAt(Vec2I(0, 0)));
    }
}

TEST(MatrixBoardView, Move) {
    World world;
    auto parentNode = MAKE<WorldNode>();

    auto sut = MAKE<MatrixBoardView>(Vector2{100, 100}, Vec2I{2, 2});
    parentNode->Add(sut);

    {
        auto node = MAKE<WorldNode>();
        parentNode->Add(node);

        VectorList<String> shape{"**"};
        auto handler = MAKE<MatrixPieceHandler>(Vec2I{0, 0}, shape);
        node->Add(handler);

        sut->Put(*handler, handler->startOrigin);
        EXPECT_EQ(Vec2I(0, 0), handler->piece->Origin());
        EXPECT_EQ(PJ::MatrixBoardView::MoveResult::Success, sut->Move(handler->piece, MapDirection::South, {}));
        EXPECT_EQ(Vec2I(0, 1), handler->piece->Origin());
        EXPECT_EQ(PJ::MatrixBoardView::MoveResult::Blocked, sut->Move(handler->piece, MapDirection::South, {}));
        EXPECT_EQ(Vec2I(0, 1), handler->piece->Origin());
    }
}

TEST(MatrixBoardView, MoveAnimated) {
    World world;
    auto parentNode = MAKE<WorldNode>();

    auto sut = MAKE<MatrixBoardView>(Vector2{100, 100}, Vec2I{2, 2});
    parentNode->Add(sut);

    {
        auto node = MAKE<WorldNode>();
        parentNode->Add(node);

        VectorList<String> shape{"**"};
        auto handler = MAKE<MatrixPieceHandler>(Vec2I{0, 0}, shape);
        node->Add(handler);

        sut->Put(*handler, handler->startOrigin);
        EXPECT_EQ(Vec2I(0, 0), handler->piece->Origin());
        MatrixBoardView::MakeAnimatorFunc func = [](Vector3 start, Vector3 end, WorldNode& target) {
            auto interpolateFunc = InterpolateFuncs::Make(start, end);
            SetBindingFunc<Vector3> binding;
            return NEW<Animator<Vector3>>(Animator<Vector3>::Config{ .interpolateFunc = interpolateFunc, .duration = 3, .binding = binding});
        };
        EXPECT_EQ(PJ::MatrixBoardView::MoveResult::Success, sut->Move(handler->piece, MapDirection::South, func));
        EXPECT_TRUE(handler->IsAnimating());
        node->OnUpdate(TimeSlice(3));
        EXPECT_FALSE(handler->IsAnimating());
    }
}
