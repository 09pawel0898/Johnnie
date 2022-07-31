#pragma once

#include <Engine/Gui.hpp>
#include "ImGui/ImGuiWidgetBase.hpp"

enum class ConsoleLogActions : uint8_t
{

};

class ConsoleLogWidget : ImGuiWidgetBase<ConsoleLogActions>
{
public:
	virtual void OnRenderGui(void) override;

public:
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.

    ConsoleLogWidget();

    void Clear();
    void AddLog(const char* fmt, ...);
    void Draw(const char* title, bool* p_open = nullptr);
};
