#pragma once
#include <string_view>


class Shader {
public:
	Shader(std::string_view vShaderPath, std::string_view fShaderPath);

	void Begin();

	void End();
};