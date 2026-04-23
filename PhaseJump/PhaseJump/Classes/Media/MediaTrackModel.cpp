#include "MediaTrackModel.h"

using namespace std;
using namespace PJ;

MediaTrackModel::InsertResult MediaTrackModel::Insert(SP<MediaClipModel> clip) {
    InsertResult result{ .insertedClip = clip };
    GUARDR(clip && clip->duration > 0, result)

    float const newStart = clip->start;
    float const newEnd = clip->End();

    VectorList<SP<MediaClipModel>> keptClips;
    keptClips.reserve(clips.size() + 1);

    for (auto& existing : clips) {
        float const existingStart = existing->start;
        float const existingEnd = existing->End();

        // Case 1: no overlap — keep as-is
        if (existingEnd <= newStart || existingStart >= newEnd) {
            keptClips.push_back(existing);
            continue;
        }

        // Case 2: new clip entirely covers existing — remove
        if (newStart <= existingStart && newEnd >= existingEnd) {
            result.removedClips.push_back(existing);
            continue;
        }

        // Case 3: existing entirely contains new clip (strict) — split into left + right
        if (existingStart < newStart && existingEnd > newEnd) {
            auto rightClip = MAKE<MediaClipModel>();
            rightClip->id = existing->id;
            rightClip->start = newEnd;
            rightClip->duration = existingEnd - newEnd;
            rightClip->sourceStart = existing->sourceStart + (newEnd - existingStart);

            // Shorten existing into left half
            existing->duration = newStart - existingStart;

            keptClips.push_back(existing);
            keptClips.push_back(rightClip);

            result.modifiedClips.push_back(existing);
            result.splitClips.push_back(rightClip);
            continue;
        }

        // Case 4: overlap on the left side of existing — trim existing from the left
        if (existingStart >= newStart && existingEnd > newEnd) {
            float const trimmed = newEnd - existingStart;
            existing->sourceStart += trimmed;
            existing->start = newEnd;
            existing->duration = existingEnd - newEnd;

            keptClips.push_back(existing);
            result.modifiedClips.push_back(existing);
            continue;
        }

        // Case 5: overlap on the right side of existing — trim existing from the right
        if (existingStart < newStart && existingEnd <= newEnd) {
            existing->duration = newStart - existingStart;

            keptClips.push_back(existing);
            result.modifiedClips.push_back(existing);
            continue;
        }
    }

    keptClips.push_back(clip);

    // Keep clips sorted by start time so lookups are predictable.
    std::sort(keptClips.begin(), keptClips.end(), [](auto const& a, auto const& b) {
        return a->start < b->start;
    });

    clips = std::move(keptClips);
    return result;
}
