#pragma once

#include "ObservedValue.h"
#include "StreamWindow.h"
#include <optional>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    /// Model for a multitrack timeline view.
    /// Tracks the total duration and the currently viewed time window.
    class MediaTimelineModel {
    public:
        using This = MediaTimelineModel;

        /// Total duration of the timeline, in seconds.
        ObservedValue<float> duration{ 0.0f };

        /// Viewable time window. When unset, the window covers the full duration.
        ObservedValue<std::optional<StreamWindow>> window{ std::optional<StreamWindow>{} };

        /// @return Returns the effective window. If no window is set, the window is the total
        /// duration.
        StreamWindow Window() const {
            auto value = window.Value();
            if (value) {
                return *value;
            }
            return StreamWindow(0.0f, duration.Value());
        }
    };
} // namespace PJ
