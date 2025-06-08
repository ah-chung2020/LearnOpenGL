#pragma once

#include "Camera.h"


class OrthoGraphicCamera : public BaseCamera {
public:
	glm::mat4 GetProjectiveMatrix() override;
};