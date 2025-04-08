#include "shader.h"
#include "fstream"
#include "sstream"

Shader::Shader(std::string_view vShaderPath, std::string_view fShaderPath) {

	std::string vShaderContent;
	std::string fShaderContent;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(vShaderPath.data());
		fShaderFile.open(fShaderPath.data());


		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vShaderContent = vShaderStream.str();
		fShaderContent = fShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		
	}
}

void Shader::Begin() {

}

void Shader::End() {


}