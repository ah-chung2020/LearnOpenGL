#pragma once


#include "Camera.h"


class PerspectiveCamera : public BaseCamera {
public:

public:
	glm::mat4 GetProjectiveMatrix() override;
};