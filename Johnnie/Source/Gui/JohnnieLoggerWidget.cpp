#include "JohnnieLoggerWidget.hpp"

#include "Log/Log.hpp"
#include <regex>

void WJohnnieLoggerWidget::OnRenderGui(void)
{
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    
    if (!ImGui::Begin("Output Log"))
    {
        ImGui::End();
        return;
    }

    if (ImGui::BeginPopup("Options"))
    {
        ImGui::Checkbox("Auto-scroll", &AutoScroll);
        ImGui::EndPopup();
    }

    if (ImGui::Button("Options"))
    {
        ImGui::OpenPopup("Options");
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear"))
    {
        TextBuf.Clear();
    }

    ImGui::SameLine();
    if (ImGui::Button("Copy"))
    {
        ImGui::LogToClipboard();
    }
    ImGui::SameLine();
    Filter.Draw("Filter", -100.0f);

    ImGui::Separator();
    ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        for (auto const& [logMsg, color] : TextBuf.Lines)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, color);
            if (Filter.IsActive())
            {
                if (Filter.PassFilter(logMsg.c_str(), logMsg.c_str() + logMsg.length()))
                {
                    ImGui::Text(logMsg.c_str());
                }
            }
            else
            {
                ImGui::Text(logMsg.c_str());
            }
            ImGui::PopStyleColor();
        }

        ImGui::PopStyleVar();

        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        {
            ImGui::SetScrollHereY(1.0f);
        }
    }
    ImGui::EndChild();
    ImGui::End();
}

WJohnnieLoggerWidget::WJohnnieLoggerWidget(void)
{
    AutoScroll = true;
    SetTickEnabled(false);

    InitLoggerSink();
}

void WJohnnieLoggerWidget::InitLoggerSink(void)
{
    std::shared_ptr<OutputLogSink_mt> outputLogSink = std::make_shared<OutputLogSink_mt>();

    OnCustomOutputLoggerSink cusatomOutputLoggerSinkDelegate;
    cusatomOutputLoggerSinkDelegate.BindRaw(this, &WJohnnieLoggerWidget::AddLog);
    outputLogSink->BindCustomLoggerFunction(MoveTemp(cusatomOutputLoggerSinkDelegate));
    
    DEFINE_OUTPUT_LOG_SINK(std::move(outputLogSink));
}

static LogManager::Verbosity GetVerbosityAndReplace(std::string& LogMsg)
{
    auto tryReplaceVerbosity = [&LogMsg](std::string_view ToReplace, std::string_view ReplaceWith) -> bool
    {
        try
        {
            LogMsg.replace(LogMsg.find(ToReplace), ReplaceWith.length(), ReplaceWith);
            return true;
        }
        catch (std::exception Ex)
        {
            return false;
        }    
    };

    if (tryReplaceVerbosity("[trace]:", "[Trace]:"))
    {
        return LogManager::Verbosity::Trace;
    }
    else if(tryReplaceVerbosity("[info]:", "[Info]:"))
    {
        return LogManager::Verbosity::Info;
    } 
    else if(tryReplaceVerbosity("[warning]:", "[Warning]:"))
    {
        return LogManager::Verbosity::Warning;
    }
    else if(tryReplaceVerbosity("[error]:", "[Error]: "))
    {
        return LogManager::Verbosity::Error;
    }
    return LogManager::Verbosity::Trace;
}

void WJohnnieLoggerWidget::AddLog(std::string const& LogMsg)
{
    static std::unordered_map<LogManager::Verbosity, ImVec4> colors =
    {
        {LogManager::Verbosity::Trace,     ImVec4(255,255,255,255)},
        {LogManager::Verbosity::Info,      ImVec4(0,255,0, 255)},
        {LogManager::Verbosity::Warning,   ImVec4(255,255,0, 255)},
        {LogManager::Verbosity::Error,     ImVec4(255,0,0,255)}
    };

    std::string fmtLogMsg = LogMsg;

    /** Format verbosity and assign color */
    ImVec4 color = colors[GetVerbosityAndReplace(fmtLogMsg)];

    /** Append new log */
    TextBuf.Lines.emplace_back(std::make_pair(fmtLogMsg, std::move(color)));
}
