#pragma once
#include <string>

enum class LogLevel {
    Error = 0,
    Info = 1,
    Debug = 2
};

class Logger {
public:
    static void Init(const std::string& filename = "logs/debug.log", LogLevel level = LogLevel::Info);
    static void Error(const std::string& message);
    static void Error(const std::wstring& message);
    static void Info(const std::string& message);
    static void Info(const std::wstring& message);
    static void Debug(const std::string& message);
    static void Debug(const std::wstring& message);
    static void Close();
    static void SetLevel(LogLevel level);
    static LogLevel GetLevel();
private:
    static LogLevel currentLevel;
    static void LogInternal(LogLevel level, const std::string& message);
    static void LogInternal(LogLevel level, const std::wstring& message);
};