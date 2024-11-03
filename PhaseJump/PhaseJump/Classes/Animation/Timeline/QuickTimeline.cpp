#include "QuickTimeline.h"
#include "StandardRandom.h"

using namespace std;
using namespace PJ;

using This = QuickTimeline;

QuickTimeline::QuickTimeline(Timeline& timeline, WorldNode& node) :
    timeline(timeline),
    node(node) {}

This& QuickTimeline::ScaleAxis(Axis axis, VectorList<float> const& keys) {
    String trackId;
    switch (axis) {
    case Axis::X:
        trackId = "scale.x";
        break;
    case Axis::Y:
        trackId = "scale.y";
        break;
    case Axis::Z:
        trackId = "scale.z";
        break;
    }

    for (size_t i = 0; i < keys.size(); i++) {
        float key = keys[i];

        float normalPos = (float)i / (float)(keys.size() - 1);
        float time = normalPos * timeline.Duration();
        timeline.AddKeyframe<float>(trackId, time, key);
    }

    return BuildTrack<float>(trackId, keys, [this, trackId, axis](auto& node) {
        timeline.SetTrackBinding<float>(trackId, [&node, axis](float value) {
            node.transform.SetAxisScale(axis, value);
        });
    });
}

This& QuickTimeline::Offset(VectorList<Vector3> const& keys, KeyedTimeType keyedTimeType) {
    String trackId{ "offset" };

    TimeTrack<Vector3>::Config trackConfig{ .id = trackId, .keyedTimeType = keyedTimeType };
    UP<PJ::SomeTimeTrack> track = NEW<TimeTrack<Vector3>>(trackConfig);
    timeline.Add(track);

    return BuildTrack<Vector3>(trackId, keys, [this, trackId](auto& node) {
        timeline.SetTrackBinding<Vector3>(trackId, [&node](Vector3 value) {
            node.transform.Value().offset = value;
        });
    });
}

This& QuickTimeline::MoveAxis(
    Axis axis, VectorList<float> const& _keys, AnimatePositionType positionType
) {
    String trackId;
    switch (axis) {
    case Axis::X:
        trackId = "move.x";
        break;
    case Axis::Y:
        trackId = "move.y";
        break;
    case Axis::Z:
        trackId = "move.z";
        break;
    }

    VectorList<float> keys = Map<float>(_keys, [&](auto key) {
        switch (positionType) {
        case AnimatePositionType::Relative:
            key += node.transform.PositionAxisValue(axis);
            break;
        default:
            break;
        }

        return key;
    });

    return BuildTrack<float>(trackId, keys, [this, trackId, axis](auto& node) {
        timeline.SetTrackBinding<float>(trackId, [&node, axis](float value) {
            node.transform.SetAxisPosition(axis, value);
        });
    });
}

This& QuickTimeline::MoveX(VectorList<float> const& keys, AnimatePositionType positionType) {
    return MoveAxis(Axis::X, keys, positionType);
}

This& QuickTimeline::MoveY(VectorList<float> const& keys, AnimatePositionType positionType) {
    return MoveAxis(Axis::Y, keys, positionType);
}

This& QuickTimeline::MoveZ(VectorList<float> const& keys, AnimatePositionType positionType) {
    return MoveAxis(Axis::Z, keys, positionType);
}

This& QuickTimeline::ScaleX(VectorList<float> const& keys) {
    return ScaleAxis(Axis::X, keys);
}

This& QuickTimeline::ScaleY(VectorList<float> const& keys) {
    return ScaleAxis(Axis::Y, keys);
}

This& QuickTimeline::ScaleZ(VectorList<float> const& keys) {
    return ScaleAxis(Axis::Z, keys);
}

This& QuickTimeline::Wobble2D(float baseScale, float scaleDelta) {
    float minScale = baseScale - scaleDelta;
    float maxScale = baseScale + scaleDelta;

    return ScaleX({ minScale, baseScale, maxScale, baseScale, minScale })
        .ScaleY({ maxScale, baseScale, minScale, baseScale, maxScale });
}

This& QuickTimeline::Shake2D(int count, float maxDistance) {
    VectorList<Vector3> keys;

    StandardRandom random;

    for (int i = 0; i < count; i++) {
        float xDelta = random.Delta(maxDistance);
        float yDelta = random.Delta(maxDistance);
        float zDelta = random.Delta(maxDistance);

        keys.push_back({ xDelta, yDelta, zDelta });
    }

    return Offset(keys, KeyedTimeType::Discrete);
}
