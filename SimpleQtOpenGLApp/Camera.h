#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};
// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  0.01f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;
const float FARPLANE    =  0.1f;
const float NEARPLANE   =  100.0f;

class Camera
{

public:

    // constructor with vectors
    Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f),
            QVector3D up = QVector3D(0.0f, 1.0f, 0.0f),
            float yaw = YAW, float pitch = PITCH);

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    
    QMatrix4x4 GetViewMatrix();

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void move(Camera_Movement direction, float deltaTime);
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void rotate(float xoffset, float yoffset, bool constrainPitch = true);

     // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
     //void ProcessMouseScroll(float yoffset)
     //{
     //    Zoom -= (float)yoffset;
     //    //if (Zoom < 1.0f)
     //    //    Zoom = 1.0f;
     //    //if (Zoom > 45.0f)
     //    //    Zoom = 45.0f;
     //}

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
