#pragma once

#include "Color.h"
#include "imgui.h"
#include "ImGuiPainter.h"
#include "Tags.h"
#include "UIPlan.h"
#include "UnorderedMap.h"

/*
 RATING: 4 stars
 Tested and works. Missing push/pop id
 CODE REVIEW: 12/17/24
 */
namespace PJ {
    /// Paints a UI plan into imGui
    class ImGuiUIPlanPainter : public ImGuiPainter {
    public:
        using DrawModelFunc = std::function<void(This&, UIModel&)>;

        UIPlan& plan;

        UnorderedMap<String, DrawModelFunc> drawModelFuncs;

        ImGuiUIPlanPainter(UIPlan& plan);
    };

    /// Pushes and pops an imGui id
    struct ImGuiPushId {
        ImGuiPushId(String value) {
            ImGui::PushID(value.c_str());
        }

        ImGuiPushId(int value) {
            ImGui::PushID(value);
        }

        ImGuiPushId(void* obj) {
            ImGui::PushID(obj);
        }

        ~ImGuiPushId() {
            ImGui::PopID();
        }
    };
} // namespace PJ
