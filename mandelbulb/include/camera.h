#pragma once

#include <glad/gl.h>

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.0f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
 public:
  // camera Attributes
  glm::vec3 position;
  glm::vec3 front;  // direction
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 world_up;
  // euler Angles
  float yaw;
  float pitch;
  // camera options
  float movement_speed;
  float mouse_sensitivity;
  float zoom;

  // constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH)
      : front(glm::vec3(0.0f, 0.0f, -1.0f)),
        movement_speed(SPEED),
        mouse_sensitivity(SENSITIVITY),
        zoom(ZOOM) {
    this->position = position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;
    UpdateCameraVectors();
  }
  // constructor with scalar values
  Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y,
         float up_z, float yaw, float pitch)
      : front(glm::vec3(0.0f, 0.0f, 1.0f)),
        movement_speed(SPEED),
        mouse_sensitivity(SENSITIVITY),
        zoom(ZOOM) {
    this->position = glm::vec3(pos_x, pos_y, pos_z);
    this->world_up = glm::vec3(up_x, up_y, up_z);
    this->yaw = yaw;
    this->pitch = pitch;
    UpdateCameraVectors();
  }

  // returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 GetViewMatrix() {
    return glm::lookAt(position, position + front, up);
  }

  // processes input received from any keyboard-like input system. Accepts input
  // parameter in the form of camera defined ENUM (to abstract it from windowing
  // systems)
  void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = movement_speed * deltaTime;
    if (direction == FORWARD) position += front * velocity;
    if (direction == BACKWARD) position -= front * velocity;
    if (direction == LEFT) position -= right * velocity;
    if (direction == RIGHT) position += right * velocity;
    if (direction == UP) position -= up * velocity;
    if (direction == DOWN) position += up * velocity;
  }

  // processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset,
                            GLboolean constrainPitch = true) {
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
      if (pitch > 89.0f) pitch = 89.0f;
      if (pitch < -89.0f) pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
  }

  // processes input received from a mouse scroll-wheel event. Only requires
  // input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset) {
    zoom -= (float)yoffset;
    if (zoom < 1.0f) zoom = 1.0f;
    if (zoom > 45.0f) zoom = 45.0f;
  }

 private:
  // calculates the front vector from the Camera's (updated) Euler Angles
  void UpdateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 local_front;
    local_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    local_front.y = sin(glm::radians(pitch));
    local_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(local_front);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(
        front, world_up));  // normalize the vectors, because their length gets
                            // closer to 0 the more you look up or down which
                            // results in slower movement.
    up = glm::normalize(glm::cross(right, front));
  }
};