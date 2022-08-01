#pragma once

#include <Engine/Gui.hpp>
#include "ImGui/ImGuiWidgetBase.hpp"
#include <list>

enum class ConsoleLogActions : uint8_t
{};



class ConsoleLogWidget : ImGuiWidgetBase<ConsoleLogActions>
{
public:
    ConsoleLogWidget(void);

	virtual void OnRenderGui(void) override;

private:
    using Line = std::pair<std::string, ImVec4>;

    struct TextBuffer
    {
        std::list<Line> Lines;

        FORCEINLINE void Clear(void)
        {
            Lines.clear();
        }
    };

    TextBuffer      TextBuf;
    ImGuiTextFilter Filter;

private:
    /** Properties */
    bool AutoScroll; 

private:
    void InitLoggerSink(void);
    void AddLog(std::string const& LogMsg);
};
