#include "OrthoGraphicCamera.h"

OrthoGraphicCamera::~OrthoGraphicCamera()
{
}

glm::mat4 OrthoGraphicCamera::GetProjectiveMatrix()
{
	return glm::ortho(
		m_left, m_right, 
		m_bottom, m_top, 
		m_zNear, m_zFar
	);
}
