#pragma once

enum class SHADERTYPE {

	TYPE_NONE,
	TYPE_VERTEX,
	TYPE_FRAGMENT,
	TYPE_COMPUTE,
};

class ShaderBase {
public:
	ShaderBase()

private:
	SHADERTYPE m_type = SHADERTYPE::TYPE_NONE;
};

class VertexShader : public ShaderBase {
private:
	SHADERTYPE m_type = SHADERTYPE::TYPE_VERTEX;
};

class FragmentShader : public ShaderBase {
private:
	SHADERTYPE m_type = SHADERTYPE::TYPE_FRAGMENT;
};

class ComputeShader : public ShaderBase {
private:
	SHADERTYPE m_type = SHADERTYPE::TYPE_COMPUTE;
};