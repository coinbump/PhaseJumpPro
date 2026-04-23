#include "MediaClipModel.h"
#include "MediaTrackModel.h"
#include "gtest/gtest.h"

using namespace PJ;

namespace {
    SP<MediaClipModel> MakeClip(float start, float duration, String id = {}) {
        auto clip = MAKE<MediaClipModel>(start, duration);
        clip->id = id;
        return clip;
    }
} // namespace

TEST(MediaTrackModel, InsertIntoEmptyAddsClip) {
    MediaTrackModel track;
    auto clip = MakeClip(0, 10);

    auto result = track.Insert(clip);

    ASSERT_EQ(1u, track.Clips().size());
    EXPECT_EQ(clip, track.Clips()[0]);
    EXPECT_EQ(clip, result.insertedClip);
    EXPECT_TRUE(result.splitClips.empty());
    EXPECT_TRUE(result.modifiedClips.empty());
    EXPECT_TRUE(result.removedClips.empty());
}

TEST(MediaTrackModel, InsertNonOverlappingKeepsBoth) {
    MediaTrackModel track;
    auto first = MakeClip(0, 5);
    track.Insert(first);

    auto second = MakeClip(10, 5);
    auto result = track.Insert(second);

    ASSERT_EQ(2u, track.Clips().size());
    EXPECT_TRUE(result.modifiedClips.empty());
    EXPECT_TRUE(result.removedClips.empty());
    EXPECT_TRUE(result.splitClips.empty());
}

TEST(MediaTrackModel, InsertSplitsContainingClip) {
    MediaTrackModel track;
    auto big = MakeClip(0, 120, "big");
    big->sourceStart = 0;
    track.Insert(big);

    auto small = MakeClip(50, 10, "small");
    auto result = track.Insert(small);

    ASSERT_EQ(3u, track.Clips().size());
    ASSERT_EQ(1u, result.splitClips.size());
    ASSERT_EQ(1u, result.modifiedClips.size());
    EXPECT_TRUE(result.removedClips.empty());

    // Left half: original clip shortened to [0, 50)
    EXPECT_FLOAT_EQ(0, big->start);
    EXPECT_FLOAT_EQ(50, big->duration);

    // Right half: new clip starting at 60
    auto right = result.splitClips[0];
    EXPECT_EQ("big", right->id);
    EXPECT_FLOAT_EQ(60, right->start);
    EXPECT_FLOAT_EQ(60, right->duration);
    EXPECT_FLOAT_EQ(60, right->sourceStart);
}

TEST(MediaTrackModel, InsertOverwritesCoveredClip) {
    MediaTrackModel track;
    auto small = MakeClip(10, 5, "small");
    track.Insert(small);

    auto big = MakeClip(0, 30, "big");
    auto result = track.Insert(big);

    ASSERT_EQ(1u, track.Clips().size());
    EXPECT_EQ(big, track.Clips()[0]);
    ASSERT_EQ(1u, result.removedClips.size());
    EXPECT_EQ(small, result.removedClips[0]);
}

TEST(MediaTrackModel, InsertTrimsLeftOverlap) {
    MediaTrackModel track;
    // Existing: [10, 30)
    auto existing = MakeClip(10, 20, "existing");
    existing->sourceStart = 0;
    track.Insert(existing);

    // New: [5, 15) — overlaps the left edge of existing
    auto newer = MakeClip(5, 10, "newer");
    auto result = track.Insert(newer);

    ASSERT_EQ(2u, track.Clips().size());
    ASSERT_EQ(1u, result.modifiedClips.size());
    EXPECT_EQ(existing, result.modifiedClips[0]);

    // Existing is now [15, 30)
    EXPECT_FLOAT_EQ(15, existing->start);
    EXPECT_FLOAT_EQ(15, existing->duration);
    // sourceStart advances by the trimmed amount
    EXPECT_FLOAT_EQ(5, existing->sourceStart);
}

TEST(MediaTrackModel, InsertTrimsRightOverlap) {
    MediaTrackModel track;
    // Existing: [10, 30)
    auto existing = MakeClip(10, 20, "existing");
    track.Insert(existing);

    // New: [25, 40) — overlaps the right edge of existing
    auto newer = MakeClip(25, 15, "newer");
    auto result = track.Insert(newer);

    ASSERT_EQ(2u, track.Clips().size());
    ASSERT_EQ(1u, result.modifiedClips.size());

    // Existing is now [10, 25)
    EXPECT_FLOAT_EQ(10, existing->start);
    EXPECT_FLOAT_EQ(15, existing->duration);
}

TEST(MediaTrackModel, InsertAcrossMultipleClips) {
    MediaTrackModel track;
    auto a = MakeClip(0, 10, "a");
    auto b = MakeClip(10, 10, "b");
    auto c = MakeClip(20, 10, "c");
    track.Insert(a);
    track.Insert(b);
    track.Insert(c);

    // Insert a clip that overlaps right of a, all of b, left of c
    auto middle = MakeClip(5, 20, "middle");
    auto result = track.Insert(middle);

    // Expected: a trimmed, b removed, c trimmed, middle added -> 3 clips
    ASSERT_EQ(3u, track.Clips().size());
    ASSERT_EQ(1u, result.removedClips.size());
    EXPECT_EQ(b, result.removedClips[0]);
    EXPECT_EQ(2u, result.modifiedClips.size());

    EXPECT_FLOAT_EQ(0, a->start);
    EXPECT_FLOAT_EQ(5, a->duration);

    EXPECT_FLOAT_EQ(25, c->start);
    EXPECT_FLOAT_EQ(5, c->duration);
}

TEST(MediaTrackModel, InsertClipsStaySortedByStart) {
    MediaTrackModel track;
    track.Insert(MakeClip(30, 5));
    track.Insert(MakeClip(0, 5));
    track.Insert(MakeClip(15, 5));

    auto const& clips = track.Clips();
    ASSERT_EQ(3u, clips.size());
    EXPECT_FLOAT_EQ(0, clips[0]->start);
    EXPECT_FLOAT_EQ(15, clips[1]->start);
    EXPECT_FLOAT_EQ(30, clips[2]->start);
}
