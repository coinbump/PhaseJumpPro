#include "SomeImGuiPainter.h"
#include "imgui.h"

using namespace std;
using namespace PJ;

void SomeImGuiPainter::Paint() {
    Begin();
    Draw();
    DrawChildren();
    End();
}

void SomeImGuiPainter::Begin() {
    GUARD(beginFunc)
    beginFunc(*this);
}

void SomeImGuiPainter::Draw() {
    GUARD(drawFunc)
    drawFunc(*this);
}

void SomeImGuiPainter::DrawChildren() {
    for (auto& child : tree.Children()) {
        child->Paint();
    }
}

void SomeImGuiPainter::End() {
    GUARD(endFunc)
    endFunc(*this);
}

// MARK: - ImGuiWindowPainter

ImGuiWindowPainter::ImGuiWindowPainter(String _name, bool* _isVisible) :
    name(_name),
    isVisible(_isVisible) {
    GUARD(isVisible)

    beginFunc = [this](auto& painter) { ImGui::Begin(name.c_str(), isVisible); };
    endFunc = [](auto& painter) { ImGui::End(); };
}

// MARK: - ImGuiWindowPainter

ImGuiChildPainter::ImGuiChildPainter(String _name) :
    name(_name) {
    beginFunc = [this](auto& painter) { ImGui::BeginChild(name.c_str()); };
    endFunc = [](auto& painter) { ImGui::EndChild(); };
}
