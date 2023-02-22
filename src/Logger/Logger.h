#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>


enum LogType {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

struct LogEntry {
    LogType type;
    std::string logline;
};

class Logger {
    private:
	static void Save();
    public:
	static std::vector<LogEntry> loglines;
	static void Log(const std::string& message);
	static void Err(const std::string& message);
	static void Warn(const std::string& message);
};

#endif
