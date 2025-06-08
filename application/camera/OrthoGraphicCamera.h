#pragma once

#include "Camera.h"


class OrthoGraphicCamera : public BaseCamera {

public:
	float m_left;
	float m_right;
	float m_top;
	float m_bottom;
	float m_zNear;
	float m_zFar;

public:
	~OrthoGraphicCamera();
	glm::mat4 GetProjectiveMatrix() override;
};