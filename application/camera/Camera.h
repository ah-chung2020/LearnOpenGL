#pragma once

#include "../Base.h"


class BaseCamera {

public:
	glm::vec3 m_position;
	glm::vec3 m_up;
	glm::vec3 m_right;

public:
	BaseCamera();
	virtual ~BaseCamera();

	glm::mat4 GetViewMatrix();

	virtual glm::mat4 GetProjectiveMatrix() = 0;
};