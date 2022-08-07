#pragma once

#include "spdlog/sinks/base_sink.h"
#include <functional>

template<typename Mutex>
class OutputLogSink : public spdlog::sinks::base_sink<Mutex>
{
private:
    using CustomOutputLoggerFunc = std::function<void(std::string const&)>;

    CustomOutputLoggerFunc m_CustomOutputLogFunc;

protected:
    void sink_it_(const spdlog::details::log_msg& Msg) override
    {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(Msg, formatted);
        m_CustomOutputLogFunc(fmt::to_string(formatted));
    }

    void flush_() override
    {}

public:
    void BindCustomLoggerFunction(CustomOutputLoggerFunc CustomOutputLoggerFunc)
    {
        m_CustomOutputLogFunc = std::move(CustomOutputLoggerFunc);
    }
};

#include "spdlog/details/null_mutex.h"
#include <mutex>

using OutputLogSink_mt = OutputLogSink<std::mutex>;
using OutputLogSink_st = OutputLogSink<spdlog::details::null_mutex>;