#pragma once

#include "SDKIncludes.h"

namespace Example {
    /// Demonstrates each `EaseFunc`
    class EaseScene : public PJ::Scene {
    public:
        using Base = PJ::Scene;
        using This = EaseScene;

        struct EaseEntry {
            PJ::String name;
            PJ::EaseFunc func;
        };

        /// Currently-selected ease func (defaults to `linear`).
        PJ::ObservedValue<PJ::EaseFunc> selectedEaseFunc{ PJ::EaseFuncs::linear };

        /// Ease functions available for selection, in the order shown in the UI.
        PJ::VectorList<EaseEntry> easeFuncs;

        /// Timer duration for the animated circle, in seconds.
        float timerDuration = 2.0f;

        EaseScene();

        // MARK: Scene

        void LoadInto(PJ::WorldNode& root) override;

    protected:
        /// Node that hosts the ImRenderer; ease-change triggers a new Timer on its updatables.
        PJ::WorldNode* renderNode{};

        /// 0-1 progress driven by the Timer; the red circle reads from here each frame.
        float progress{};

        /// Resets progress and restarts the timer on selection change.
        void OnEaseFuncChanged();
    };
} // namespace Example
