#include "ImGuiGLRenderProcessor.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"

using namespace std;
using namespace PJ;

void ImGuiGLRenderProcessor::Process(Phase phase) {
    GUARD_LOG(imGuiContext, "Missing imGuiContext")
    GUARD_LOG(nullptr == phase.cameraModel, "ERROR. Don't process imGui for each camera")

    if (phase.id == RenderPhase::RenderPassStartPrepare) {
        ImGui::SetCurrentContext(imGuiContext);

        // Start the imGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    } else if (phase.id == RenderPhase::RenderPassPresentPrepare) {
        // Render the imGui frame
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
