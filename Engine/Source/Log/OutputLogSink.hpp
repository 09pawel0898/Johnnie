#pragma once

#include "Core/CoreMinimal.hpp"
#include "Utilities/Delegates.hpp"

#include "spdlog/sinks/base_sink.h"

DECLARE_DELEGATE(OnCustomOutputLoggerSink,std::string const&);

template<typename Mutex>
class OutputLogSink : public spdlog::sinks::base_sink<Mutex>
{
private:
    OnCustomOutputLoggerSink m_OnCustomOutputLoggerSink;

protected:
    void sink_it_(const spdlog::details::log_msg& Msg) override
    {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(Msg, formatted);
        m_OnCustomOutputLoggerSink.ExecuteIfBound(fmt::to_string(formatted));
    }

    void flush_() override
    {}

public:
    void BindCustomLoggerFunction(OnCustomOutputLoggerSink OnCustomOutputLoggerSink)
    {
        m_OnCustomOutputLoggerSink = MoveTemp(OnCustomOutputLoggerSink);
    }
};

#include "spdlog/details/null_mutex.h"
#include <mutex>

using OutputLogSink_mt = OutputLogSink<std::mutex>;
using OutputLogSink_st = OutputLogSink<spdlog::details::null_mutex>;