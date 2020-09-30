#include "ShaderHandler.h"

std::map<std::string, GLuint> ShaderHandler::programs = std::map<std::string, GLuint>();
std::unique_ptr<ShaderHandler> ShaderHandler::instance = nullptr;

ShaderHandler::ShaderHandler() {

}

ShaderHandler::~ShaderHandler() {
	OnDestroy();
}

void ShaderHandler::CreateProgram(const std::string& shaderName_, const std::string& vertexShaderFilename_, const std::string& fragmentShaderFilename_) {
	std::string vertexShaderCode = ReadShader(vertexShaderFilename_);
	std::string fragmentShaderCode = ReadShader(fragmentShaderFilename_);
	if (vertexShaderCode == "" || fragmentShaderCode == "") {
		return;
	}

	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderCode, shaderName_);
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode, shaderName_);
	if (vertexShader == 0 || fragmentShader == 0) {
		return;
	}

	GLint linkResult = 0;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
	if (!linkResult) {
		GLint infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<char> programLog(infoLogLength);
		glGetProgramInfoLog(program, infoLogLength, NULL, &programLog[0]);

		std::string programString(programLog.begin(), programLog.end());
		DebugLogger::Error("Failed to link shader " + shaderName_ + ". Error:\n" + programString, "ShaderHandler.cpp", __LINE__);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(program);
		return;
	}
	programs[shaderName_] = program;
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

GLuint ShaderHandler::GetShader(const std::string& shaderName_) {
	if (programs.find(shaderName_) != programs.end()) {
		return programs[shaderName_];
	}
	return 0;
}

ShaderHandler* ShaderHandler::GetInstance() {
	if (instance.get() == nullptr) {
		instance.reset(new ShaderHandler);
	}
	return instance.get();
}

void ShaderHandler::OnDestroy() {
	if (programs.size() > 0) {
		for (auto p : programs) {
			glDeleteProgram(p.second);
		}
		programs.clear();
	}
}

std::string ShaderHandler::ReadShader(const std::string& filePath_) {
	std::string shaderCode = "";
	std::ifstream file;
	file.exceptions(std::iostream::badbit);
	try {
		file.open(filePath_);
		std::stringstream tempStream;
		tempStream << file.rdbuf();
		file.close();

		shaderCode = tempStream.str();
	} catch (std::ifstream::failure error_) {
		DebugLogger::Error("Could not read shader " + filePath_, "ShaderHandler.cpp", __LINE__);
		return "";
	}
	return shaderCode;
}

GLuint ShaderHandler::CreateShader(GLenum shaderType_, const std::string& source_, const std::string& shaderName_) {
	GLint compileResult = 0;
	GLuint shader = glCreateShader(shaderType_);
	const char* shaderCodePtr = source_.c_str();
	const int shaderCodeSize = source_.size();

	glShaderSource(shader, 1, &shaderCodePtr, &shaderCodeSize);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (!compileResult) {
		GLint infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::vector<char> shaderLog(infoLogLength);
		glGetShaderInfoLog(shader, infoLogLength, NULL, &shaderLog[0]);

		std::string shaderString(shaderLog.begin(), shaderLog.end());
		DebugLogger::Error("Shader couldn't be compiled " + shaderName_ + ". Error:\n" + shaderString, "ShaderHandler.cpp", __LINE__);
		return 0;
	}
	return shader;
}