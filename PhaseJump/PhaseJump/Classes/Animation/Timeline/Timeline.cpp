#include "Timeline.h"

using namespace std;
using namespace PJ;

using TrackList = Timeline::TrackList;

Timeline::Timeline(Config const& config) :
    timer(config.duration, config.cycleType) {
    timer.SetOnPlayTimeChangeFunc([this](auto& playable) { OnPlayTimeChange(); });

    OnPlayTimeChangeFunc overrideFunc = [](auto& playable) {
        Timeline& timeline = *(static_cast<Timeline*>(&playable));

        float playTime = timeline.PlayTime();
        for (auto& track : timeline.tracks) {
            track->SetPlayTime(playTime);
        }
    };
    Override(onPlayTimeChangeFunc, overrideFunc);
}

TrackList::iterator Timeline::Add(UP<SomeTimeTrack>& track) {
    return tracks.emplace(tracks.end(), std::move(track));
}

TrackList::iterator Timeline::FindTrackIterator(String id) {
    return std::find_if(tracks.begin(), tracks.end(), [=](auto& track) { return track->id == id; });
}

TrackList::const_iterator Timeline::FindTrackIterator(String id) const {
    return std::find_if(tracks.cbegin(), tracks.cend(), [=](auto const& track) {
        return track->id == id;
    });
}

SomeTimeTrack* Timeline::FindTrack(String id) const {
    auto i = FindTrackIterator(id);
    GUARDR(i != tracks.end(), {});

    return (*i).get();
}
