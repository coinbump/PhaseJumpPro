#include "EditorImGuiWorldInfoPainter.h"
#include "DevLog.h"
#include "DevTerminal.h"
#include "imgui.h"
#include "RenderWorldSystem.h"
#include "SomeWorldSystem.h"
#include "World.h"
#include <array>

using namespace std;
using namespace PJ;

namespace {
    bool isTerminalWindowOpen = false;
    bool isRenderGraphWindowOpen = false;
    std::array<char, 512> terminalInputBuffer{};
    bool shouldScrollLogToBottom = false;
    bool shouldRefocusTerminalInput = false;

    /// Render a RenderInfo node (and its children) as a CollapsingHeader tree.
    /// idCounter disambiguates headers that share the same title
    void DrawRenderInfoNode(RenderInfo const& info, int& idCounter) {
        ImGui::PushID(idCounter++);

        String headerTitle = !info.items.empty() ? info.items[0].value : String("(empty)");
        if (ImGui::CollapsingHeader(headerTitle.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (auto const& item : info.items) {
                ImGui::Text("%s: %s", item.label.c_str(), item.value.c_str());
            }

            if (!info.tree.Children().empty()) {
                ImGui::Indent();
                for (auto const& child : info.tree.Children()) {
                    GUARD_CONTINUE(child)
                    DrawRenderInfoNode(*child, idCounter);
                }
                ImGui::Unindent();
            }
        }

        ImGui::PopID();
    }

    void DrawRenderGraphWindow(World& world) {
        GUARD(isRenderGraphWindowOpen)

        ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("Render Graph", &isRenderGraphWindowOpen)) {
            ImGui::End();
            return;
        }

        auto* renderSystem = world.TypeSystem<RenderWorldSystem>();
        if (!renderSystem) {
            ImGui::Text("No render system");
            ImGui::End();
            return;
        }

        auto const* info = renderSystem->LastRenderInfo();
        if (!info) {
            ImGui::Text("No render data yet");
            ImGui::End();
            return;
        }

        int idCounter = 0;
        DrawRenderInfoNode(*info, idCounter);

        ImGui::End();
    }

    void DrawDevTerminalWindow() {
        GUARD(isTerminalWindowOpen)

        auto& io = ImGui::GetIO();
        float windowWidth = io.DisplaySize.x * 0.6f;
        float windowHeight = io.DisplaySize.y * 0.2f;
        float windowX = (io.DisplaySize.x - windowWidth) * 0.5f;
        float windowY = io.DisplaySize.y - windowHeight;

        ImGui::SetNextWindowPos(ImVec2(windowX, windowY), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);

        if (!ImGui::Begin("Dev Terminal", &isTerminalWindowOpen)) {
            ImGui::End();
            return;
        }

        if (ImGui::BeginTabBar("DevTerminalTabs")) {
            if (ImGui::BeginTabItem("Log")) {
                ImVec2 logRegion = ImVec2(0, -ImGui::GetFrameHeightWithSpacing());
                if (ImGui::BeginChild("LogScrolling", logRegion, true)) {
                    for (auto const& entry : devLog.GetHistory().Items()) {
                        ImGui::TextUnformatted(entry.c_str());
                    }

                    if (shouldScrollLogToBottom || ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                        ImGui::SetScrollHereY(1.0f);
                        shouldScrollLogToBottom = false;
                    }
                }
                ImGui::EndChild();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Terminal")) {
                float footerHeight = ImGui::GetFrameHeightWithSpacing();
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

                if (ImGui::BeginChild("TerminalScrolling", ImVec2(0, -footerHeight), true)) {
                    for (auto const& entry : devTerminal.GetHistory().Items()) {
                        ImGui::TextUnformatted(entry.c_str());
                    }
                    ImGui::SetScrollHereY(1.0f);
                }
                ImGui::EndChild();
                ImGui::PopStyleVar();

                auto submit = [&] {
                    String command(terminalInputBuffer.data());
                    GUARD(!command.empty())
                    devTerminal.RunCommand(command);
                    terminalInputBuffer[0] = '\0';
                };

                float runButtonWidth = 60.0f;
                float inputWidth = ImGui::GetContentRegionAvail().x - runButtonWidth -
                                   ImGui::GetStyle().ItemSpacing.x;
                ImGui::SetNextItemWidth(inputWidth);

                if (shouldRefocusTerminalInput) {
                    ImGui::SetKeyboardFocusHere();
                    shouldRefocusTerminalInput = false;
                }

                bool entered = ImGui::InputText(
                    "##DevTerminalInput", terminalInputBuffer.data(), terminalInputBuffer.size(),
                    ImGuiInputTextFlags_EnterReturnsTrue
                );

                ImGui::SameLine();
                bool runPressed = ImGui::Button("Run", ImVec2(runButtonWidth, 0));

                if (entered || runPressed) {
                    submit();
                    if (entered) {
                        shouldRefocusTerminalInput = true;
                    }
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }
} // namespace

EditorImGuiWorldInfoPainter::EditorImGuiWorldInfoPainter(World& _world) :
    world(_world) {

    devLog.onAddFunc = [](String const&) { shouldScrollLogToBottom = true; };

    drawFunc = [this](auto& painter) {
        if (ImGui::CollapsingHeader("World", ImGuiTreeNodeFlags_DefaultOpen)) {
            int fps = world.renderStats.SafeValue<int>("fps");

            ImGui::Checkbox("Limit render FPS", &world.isRenderRateLimited);
            ImGui::SliderFloat("World speed", &world.timeScale, 0.0f, 5.0f);

            String fpsString = MakeString(fps);
            String fpsTitle = String("FPS: ") + fpsString;
            ImGui::TextColored(
                fps < 30 ? ImVec4(1, 0, 0, 1) : ImVec4(0, 1, 0, 1), "%s", fpsTitle.c_str()
            );

            if (world.IsPaused()) {
                if (ImGui::Button("Play")) {
                    world.Play();
                }
            } else {
                if (ImGui::Button("Pause")) {
                    world.Pause();
                }
            }

            {
                int drawCount = world.renderStats.SafeValue<int>(RenderStatId::DrawCount);
                String countString = MakeString(drawCount);
                String countTitle = String("Draw Count: ") + countString;
                ImGui::Text("%s", countTitle.c_str());
            }

            {
                int nodeCount = world.renderStats.SafeValue<int>(RenderStatId::NodeCount);
                String countString = MakeString(nodeCount);
                String countTitle = String("Node Count: ") + countString;
                ImGui::Text("%s", countTitle.c_str());
            }

            if (ImGui::Button("Show Render Graph")) {
                isRenderGraphWindowOpen = true;
            }

            auto& systems = world.Systems();

            ImGui::Text("Systems");

            for (size_t i = 0; i < systems.size(); i++) {
                auto& system = systems[i];
                GUARD_CONTINUE(system->Name().size() > 0)

                ImGui::Text("%s", system->Name().c_str());
            }

            if (ImGui::Button("Show Terminal")) {
                isTerminalWindowOpen = true;
            }
        }

        DrawDevTerminalWindow();
        DrawRenderGraphWindow(world);
    };
}
