#include "Camera.h"

BaseCamera::BaseCamera() :
    m_position{0.0f, 0.0f, 2.0f},
    m_up{0.0f, 1.0f, 0.0f},
    m_right{1.0f, 0.0f, 0.0f}
{
}

BaseCamera::~BaseCamera()
{
}

glm::mat4 BaseCamera::GetViewMatrix()
{
    glm::vec3 front = glm::cross(m_up, m_right);
    glm::vec3 target = m_position + front;

    return glm::lookAt(m_position, target, m_up);
}
