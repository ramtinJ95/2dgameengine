#include "Logger.h"
#include <fstream>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>


std::vector<LogEntry> Logger::loglines;

std::string CurrentDateTimeToString() {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string dateTimeString(30, '\0');
    std::strftime(&dateTimeString[0], dateTimeString.size(), "%d-%b-%Y %H:%M:%S", std::localtime(&now));
    return dateTimeString;
}

void Logger::Save() {
    std::stringstream filename; 
    filename << "DIY_Engine" << ".log"; 

    std::ofstream logFile;
    logFile.open(filename.str());
    logFile << "Game Engine Logging File - " << "DIY_Engine" << "\n";

    for(auto line : loglines) {
        std::string typeName;
        switch (line.type) {
            case LogType::LOG_INFO:
                typeName = "INFO";
                break;
            case LogType::LOG_WARNING:
                typeName = "WARNING";
                break;
            case LogType::LOG_ERROR:
                typeName = "ERROR";
                break;
        }
        logFile << "[" << typeName << "]" << line.logline << "\n";
    }
    logFile.close();
}

void Logger::Log(const std::string& message){
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.logline = "LOG: [" + CurrentDateTimeToString() + "]: " + message;
    std::cout <<"\033[32m" + logEntry.logline + "\033[m"<< std::endl;
    loglines.push_back(logEntry);
    Logger::Save();
};
void Logger::Err(const std::string& message){
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.logline = "ERR: [" + CurrentDateTimeToString() + "]: " + message;
    std::cout <<"\033[31m" + logEntry.logline + "\033[m"<< std::endl;
    loglines.push_back(logEntry);
}
void Logger::Warn(const std::string& message){
    LogEntry logEntry;
    logEntry.type = LOG_WARNING;
    logEntry.logline = "WARN: [" + CurrentDateTimeToString() + "]: " + message;
    std::cout <<"\033[33m" + logEntry.logline + "\033[m"<< std::endl;
    loglines.push_back(logEntry);
}
