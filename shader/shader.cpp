#include "shader.h"
#include "shader.h"
#include "shader.h"
#include "fstream"
#include "sstream"

Shader::Shader(std::string_view vShaderPath, std::string_view fShaderPath)
	:m_shaderProgram(0)
{

	std::string vShaderContent;
	std::string fShaderContent;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	unsigned int vertexShader = 0;
	unsigned int fragmentShader = 0;
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

		const char* vShaderContentStr = vShaderContent.c_str();
		const char* fShaderContentStr = fShaderContent.c_str();

		/*
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderContentStr, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderContentStr, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		*/
		
		vertexShader = _ReadAndCompileShader(GL_VERTEX_SHADER, vShaderContentStr);
		fragmentShader = _ReadAndCompileShader(GL_FRAGMENT_SHADER, fShaderContentStr);
		
	}
	catch (std::ifstream::failure& e) {
		std::cout << "read file error!" << std::endl;
	}

	// link shaders
	int success;
	char infoLog[512];
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);
	glLinkProgram(m_shaderProgram);
	// check for linking errors
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Begin() {
	if (m_shaderProgram != 0) {
		glUseProgram(m_shaderProgram);
	}
	
}

void Shader::End() {
	glUseProgram(0);
}

void Shader::SetInt(std::string_view name, int value)
{
	// 按名字找出location
	GLint location = glGetUniformLocation(m_shaderProgram, name.data());

	if (location == -1) {
		std::cerr << "Warning: Uniform '" << name << "' not found\n";
		return;
	}

	// 按location设置值
	glUniform1i(location, value);
}

void Shader::SetMat4(std::string_view name, glm::mat4 value)
{
	GLint location = glGetUniformLocation(m_shaderProgram, name.data());
	if (location == -1) {
		std::cerr << "Warning: Uniform '" << name << "' not found\n";
		return;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

}

unsigned int Shader::_ReadAndCompileShader(int shaderType, const char* shaderContentStr)
{
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderContentStr, NULL);
	glCompileShader(shader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}
