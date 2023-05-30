#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

enum moveDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UPWARD,
    DOWN
};

class Camera
{
public:
    glm::vec3 Position  =   vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Front     =   vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Up        =   vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Right     =   normalize(cross(Front, Up));
    glm::vec3 WorldUp   =   vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 defaultPosition = vec3(0.0f, 0.0f, 0.0f);

    //vec3 cameraDirection = normalize(cameraPos - cameraTarget);       == - cameraFront
    //vec3 cameraRight = normalize(cross(cameraUp, cameraDirection));
    //mat4 view = lookAt(cameraPos, cameraTarget, cameraUp);

    float Pitch = 0, Yaw = 0;

    float MovementSpeed = 0.0f;
    float MouseSensitivity = 0.0f;
    float Fov = 0.0f;

    bool isFPScamera;
    bool constrainPitch;

    Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    mat4 getViewMatrix();
    
    void updateCamera();
    void moveCamera(float deltaTime, moveDirection direction);
    void rotateCamera(float xoffset, float yoffset);
    void zoomCamera(float yoffset);
    void setFPScamera();
    void setconstrainPitch();
    void resetcamera();
};

#endif