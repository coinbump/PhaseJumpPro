#include "EditorImGuiCommandHistoryPainter.h"
#include "CommandHistory.h"
#include "imgui.h"

using namespace std;
using namespace PJ;

EditorImGuiCommandHistoryPainter::EditorImGuiCommandHistoryPainter(CommandHistory& _commandHistory
) :
    commandHistory(_commandHistory) {
    drawFunc = [this](auto& painter) {
        if (ImGui::CollapsingHeader("Command history", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Button("Undo")) {
                commandHistory.Undo();
            }
            ImGui::SameLine();
            if (ImGui::Button("Redo")) {
                commandHistory.Redo();
            }

            for (auto& command : commandHistory.Commands()) {
                auto name = command->Name();
                GUARD_CONTINUE(!IsEmpty(name));

                switch (command->State()) {
                case SomeCommand::StateType::Complete:
                    ImGui::Text("%s", name.c_str());
                    break;
                default:
                    ImGui::TextColored(ImVec4(0.3, 0.3, 0.3, 1), "%s", name.c_str());
                    break;
                }
            }
        }
    };
}
