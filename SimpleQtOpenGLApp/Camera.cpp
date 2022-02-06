#include "stdafx.h"

#include "camera.h"

Camera::Camera(QVector3D position,QVector3D up ,
       float yaw , float pitch ):
    m_position(position),
    m_up(up),
    m_yaw(yaw),
    m_pitch(pitch),
    m_front(-position),
    m_speed(SPEED),
    m_sensitivity(SENSITIVITY),
    m_zoom(ZOOM),
    m_near(NEARPLANE),
    m_far(FARPLANE)
{
     updateCameraVectors();
}
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
    m_front(QVector3D(0.0f, 0.0f, -1.0f)),
    m_speed(SPEED),
    m_sensitivity(SENSITIVITY),
    m_zoom(ZOOM)
   {
       m_position =QVector3D(posX, posY, posZ);
       m_yaw = yaw;
       m_pitch = pitch;
       updateCameraVectors();
   }

QMatrix4x4 Camera::GetViewMatrix()
{
	QMatrix4x4 result;
	result.lookAt(m_position, m_position + m_front, m_up);
	return result;
}


void Camera::move(Camera::Camera_Movement direction, float deltaTime)
{
    float velocity = m_speed * deltaTime;
    if (direction == Camera::FORWARD)
        m_position += m_front * velocity;
    if (direction == Camera::BACKWARD)
        m_position -= m_front * velocity;
    if (direction == Camera::LEFT)
        m_position += QVector3D::crossProduct(m_front, m_up).normalized() * velocity;
    if (direction == Camera::RIGHT)
        m_position -= QVector3D::crossProduct(m_front, m_up).normalized() * velocity;
    if (direction == Camera::UP)
        m_position += m_up * velocity;
    if (direction == Camera::DOWN)
        m_position -= m_up * velocity;
}


void Camera::pan(float xoffset, float yoffset, float deltaTime)
{
	float velocity = m_speed * deltaTime;

    m_position.setX(m_position.x() + xoffset * velocity);
	m_position.setY(m_position.y() + yoffset * velocity);
}

void Camera::rotate(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= m_sensitivity;
    yoffset *= m_sensitivity;

    m_yaw   += xoffset;
    m_pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::updateCameraVectors()
  {
      // calculate the new Front vector
      QVector3D front;
      front.setX(cos(Radians(this->m_yaw)) * cos(Radians(this->m_pitch)));
      front.setY(sin(Radians(this->m_pitch)));
      front.setZ(sin(Radians(this->m_yaw)) * cos(Radians(this->m_pitch)));
      m_front = front.normalized();

      // also re-calculate the Right and Up vector
	  //Right = QVector3D::crossProduct(Front, Up).normalized();
	  //Up = QVector3D::crossProduct(Right, Front).normalized();
  }
