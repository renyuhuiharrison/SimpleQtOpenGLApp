#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>


// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 0.01f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float FARPLANE = 100.0f;
const float NEARPLANE = 0.1f;

class Camera
{
public:
	enum Camera_Movement : int
    {
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN,
	};

public:

    Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f),
            QVector3D up = QVector3D(0.0f, 1.0f, 0.0f),
            float yaw = YAW, float pitch = PITCH);

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    
    QMatrix4x4 GetViewMatrix();

    //相机视角移动
    void move(Camera::Camera_Movement direction, float deltaTime);
    
    //相机视角平移
    void pan(float xoffset, float yoffset, float deltaTime);

    //相机视角旋转
    void rotate(float xoffset, float yoffset, bool constrainPitch = true);

    inline float getZoom() const
    {
        return m_zoom;
    }

    inline float getNear() const
    {
        return m_near;
    }
    
	inline float getFar() const
	{
		return m_far;
	}

private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();

	float Radians(float angle)
	{
		return angle * M_PI / 180;
	}

private:
     QVector3D m_position;
     QVector3D m_front;
     QVector3D m_up;
     //QVector3D Right;
     
     // euler Angles
     float m_yaw;
     float m_pitch;

     // camera options
     float m_speed;
     float m_sensitivity;
     float m_zoom;
     float m_far;
     float m_near;

};

#endif // CAMERA_H
