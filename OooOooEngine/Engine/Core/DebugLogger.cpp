#include "DebugLogger.h"

MESSAGE_TYPE DebugLogger::curMessage = MESSAGE_TYPE::NONE;
std::string DebugLogger::outputName = "";

void DebugLogger::DebugInit() {
	outputName = "OooOooEngineLog.txt";
	std::ofstream out;
	out.open(outputName.c_str(), std::ios::out);
	out.close();
}

void DebugLogger::SetSeverity(MESSAGE_TYPE type_) {
	curMessage = type_;
}

void DebugLogger::Info(const std::string& message_, const std::string& filename_, const int line_) {
	Log(MESSAGE_TYPE::INFO, "[INFO]: " + message_, filename_, line_);
}

void DebugLogger::Trace(const std::string& message_, const std::string& filename_, const int line_) {
	Log(MESSAGE_TYPE::TRACE, "[TRACE]: " + message_, filename_, line_);
}

void DebugLogger::Warning(const std::string& message_, const std::string& filename_, const int line_) {
	Log(MESSAGE_TYPE::WARNING, "[WARNING]: " + message_, filename_, line_);
}

void DebugLogger::Error(const std::string& message_, const std::string& filename_, const int line_) {
	Log(MESSAGE_TYPE::ERROR, "[ERROR]: " + message_, filename_, line_);
}

void DebugLogger::FatalError(const std::string& message_, const std::string& filename_, const int line_) {
	Log(MESSAGE_TYPE::FATAL_ERROR, "[FATAL_ERROR]: " + message_, filename_, line_);
}

void DebugLogger::Log(const MESSAGE_TYPE type_, const std::string& message_, const std::string& filename_, const int line_) {
	if (type_ <= curMessage && curMessage > MESSAGE_TYPE::NONE) {
		std::ofstream out;
		out.open(outputName.c_str(), std::ios::app | std::ios::out);
		out << message_ << " in: " << filename_ << " on line: " << line_ << std::endl;
		out.flush();
		out.close();
	}
}
