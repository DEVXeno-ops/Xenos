#pragma once

#include <fstream>
#include <cstdarg>
#include <ctime>
#include <cstdio>
#include <mutex>

namespace xlog {

namespace LogLevel {
    enum e {
        fatal = 0,
        error,
        critical,
        warning,
        normal,
        verbose,
        count // Not a log level; used for bounds check
    };
}

static const char* sLogLevel[] = {
    "*FATAL*",
    "*ERROR*",
    "*CRITICAL*",
    "*WARNING*",
    "*NORMAL*",
    "*VERBOSE*"
};

class Logger {
public:
    ~Logger() {
        if (_output.is_open()) {
            _output << std::endl;
            _output.close();
        }
    }

    static Logger& Instance() {
        static Logger instance;
        return instance;
    }

    bool DoLog(LogLevel::e level, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        bool result = DoLogV(level, fmt, args);
        va_end(args);
        return result;
    }

    bool DoLogV(LogLevel::e level, const char* fmt, va_list args) {
        if (!_output.is_open() || level < 0 || level >= LogLevel::count)
            return false;

        std::lock_guard<std::mutex> lock(_mutex);

        char varbuf[2048] = {};
        char message[4096] = {};
        char timebuf[64] = {};

        std::time_t now = std::time(nullptr);
        std::tm stm{};
        localtime_s(&stm, &now);
        std::strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &stm);

        vsnprintf(varbuf, sizeof(varbuf), fmt, args);
        snprintf(message, sizeof(message), "%s %-10s %s", timebuf, sLogLevel[level], varbuf);

        _output << message << std::endl;
        return true;
    }

private:
    Logger() {
        _output.open("Xenos.log", std::ios::out | std::ios::app);
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream _output;
    std::mutex _mutex;
};

// Inline helper macros
#define DEFINE_LOG_FN(name, level)                      \
inline bool name(const char* fmt, ...) {                \
    va_list args;                                       \
    va_start(args, fmt);                                \
    bool result = Logger::Instance().DoLogV(level, fmt, args); \
    va_end(args);                                       \
    return result;                                      \
}

DEFINE_LOG_FN(Fatal,    LogLevel::fatal)
DEFINE_LOG_FN(Error,    LogLevel::error)
DEFINE_LOG_FN(Critical, LogLevel::critical)
DEFINE_LOG_FN(Warning,  LogLevel::warning)
DEFINE_LOG_FN(Normal,   LogLevel::normal)
DEFINE_LOG_FN(Verbose,  LogLevel::verbose)

} // namespace xlog
