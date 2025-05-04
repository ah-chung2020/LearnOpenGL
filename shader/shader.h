#pragma once
#include <iostream>
#include <string_view>

#include "../Base.h"

class Shader {
public:
	Shader(std::string_view vShaderPath, std::string_view fShaderPath);

	void Begin();

	void End();

private:
	unsigned int _ReadAndCompileShader(int shaderType, const char*& shaderContentStr);

	unsigned int m_shaderProgram;
};