#include "camera.hpp"

Camera::Camera(vec3 position, vec3 up, float yaw, float pitch):
    Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV), isFPScamera(false), constrainPitch(true)
{

    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
}

mat4 Camera::getViewMatrix() {
    return lookAt(Position, Front + Position, Up);
}

void Camera::updateCamera() {
    vec3 front = vec3(0.0f, 0.0f, 0.0f);
    front.x = cos(radians(Pitch)) * cos(radians(Yaw));
    front.y = sin(radians(Pitch));
    front.z = cos(radians(Pitch)) * sin(radians(Yaw));
    Front = normalize(front);

    Right = normalize(cross(Front, WorldUp));
    Up = normalize(cross(Right, Front));
}
void Camera::moveCamera(float deltaTime, moveDirection direction) {
    if (isFPScamera) {
        switch (direction) {
        case FORWARD:   Position += MovementSpeed * deltaTime * vec3(Front.x, 0.0, Front.z); break;
        case BACKWARD:  Position -= MovementSpeed * deltaTime * vec3(Front.x, 0.0, Front.z); break;
        case LEFT:      Position -= MovementSpeed * deltaTime * Right; break;
        case RIGHT:     Position += MovementSpeed * deltaTime * Right; break;
        case UPWARD:
        case DOWN:
        default:        Position = Position; break;
        }
    }
    else {
        switch (direction) {break;
        case FORWARD:   Position += MovementSpeed * deltaTime * Front; break;
        case BACKWARD:  Position -= MovementSpeed * deltaTime * Front; break;
        case LEFT:      Position -= MovementSpeed * deltaTime * Right; break;
        case RIGHT:     Position += MovementSpeed * deltaTime * Right; break;
        case UPWARD:    Position += MovementSpeed * deltaTime * Up; break;
        case DOWN:      Position -= MovementSpeed * deltaTime * Up; break;
        default:        Position = Position; break;
        }
    }
}

void Camera::rotateCamera(float xoffset, float yoffset) {
    Yaw += MouseSensitivity * xoffset;
    Pitch += MouseSensitivity * yoffset;

    if (constrainPitch)
    {
        if (Pitch >  89.0f)     Pitch =  89.0f;
        if (Pitch < -89.0f)     Pitch = -89.0f;
    }
    updateCamera();
}

void Camera::zoomCamera(float yoffset) {
    Fov -= yoffset;
    if (Fov <= 1.0f)    Fov = 1.0f;
    if (Fov >= 90.0f)   Fov = 90.0f;
}
void Camera::setFPScamera() {
    if (isFPScamera) {


        isFPScamera = false;
    }
    else {
        isFPScamera = true;
    }
}

void Camera::setconstrainPitch() {
    constrainPitch ? constrainPitch = false : constrainPitch = true;
}

