#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include "../Core/DebugLogger.h"
#include <glew.h>
#include <sstream>
#include <vector>
#include <map>
#include <memory>

class ShaderHandler {
private:
	static std::map<std::string, GLuint> programs;

	static std::unique_ptr<ShaderHandler> instance;
	friend std::default_delete<ShaderHandler>;
public:
	ShaderHandler(const ShaderHandler&) = delete;
	ShaderHandler(ShaderHandler&&) = delete;
	ShaderHandler& operator=(const ShaderHandler&) = delete;
	ShaderHandler& operator=(ShaderHandler&&) = delete;

	void CreateProgram(const std::string& shaderName_, const std::string& vertexShaderFilename_, const std::string& fragmentShaderFilename_);
	GLuint GetShader(const std::string& shaderName_);
	void OnDestroy();

	static ShaderHandler* GetInstance();
private:
	ShaderHandler();
	~ShaderHandler();

	std::string ReadShader(const std::string& filePath_);
	GLuint CreateShader(GLenum shaderType_, const std::string& source_, const std::string& shaderName_);
};

#endif