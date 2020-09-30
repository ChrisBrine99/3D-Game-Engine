#ifndef DEBUGLOGGER_H
#define DEBUGLOGGER_H

#include <iostream>
#include <string>
#include <fstream>

enum class MESSAGE_TYPE : unsigned short {
	NONE =				0x0000,
	FATAL_ERROR =		0x0001,
	ERROR =				0x0002,
	WARNING =			0x0003,
	TRACE =				0x0004,
	INFO =				0x0005,
};

class DebugLogger {
private:
	static MESSAGE_TYPE curMessage;
	static std::string outputName;
public:
	DebugLogger(const DebugLogger&) = delete;
	DebugLogger(DebugLogger&&) = delete;
	DebugLogger& operator=(const DebugLogger&) = delete;
	DebugLogger& operator=(DebugLogger&&) = delete;

	DebugLogger() = delete;

	static void DebugInit();
	static void SetSeverity(MESSAGE_TYPE type_);

	static void Info(const std::string& message_, const std::string& filename_, const int line_);
	static void Trace(const std::string& message_, const std::string& filename_, const int line_);
	static void Warning(const std::string& message_, const std::string& filename_, const int line_);
	static void Error(const std::string& message_, const std::string& filename_, const int line_);
	static void FatalError(const std::string& message_, const std::string& filename_, const int line_);
private:
	static void Log(const MESSAGE_TYPE type_, const std::string& message_, const std::string& filename_, const int line_);
};

#endif

