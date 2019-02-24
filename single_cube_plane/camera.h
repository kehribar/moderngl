// ----------------------------------------------------------------------------
//
//
// ----------------------------------------------------------------------------
#ifndef CAMERA_H
#define CAMERA_H

// ----------------------------------------------------------------------------
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "misc.h"

// ----------------------------------------------------------------------------
enum Camera_Movement{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

// ----------------------------------------------------------------------------
class Camera
{

  // ...
  public:
    
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    // Euler Angles
    float Yaw;
    float Pitch;
    
    // Camera options
    float Zoom;
    float MovementSpeed;
    float MouseSensitivity;

    // ...
    Camera(glm::vec3 position)
    {
      // ...
      Zoom = 45.0f;
      Pitch = 0.0f;
      Yaw = -90.0f;
      MovementSpeed = 2.5f; 
      MouseSensitivity = 0.1f;

      // ...
      Position = position;
      WorldUp  = glm::vec3(0.0f, 1.0f, 0.0f);
      
      // ...
      updateCameraVectors();
    }

    // ...
    glm::mat4 GetViewMatrix()
    {
      return glm::lookAt(Position, Position + Front, Up);
    }

    // ...
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
      float velocity = MovementSpeed * deltaTime;

      if(direction == FORWARD)
      {
        Position += Front * velocity;
      }
      else if(direction == BACKWARD)
      {
        Position -= Front * velocity;
      }
      else if(direction == LEFT)
      {
        Position -= Right * velocity;
      }
      else if(direction == RIGHT)
      {
        Position += Right * velocity;
      }
    }

    // ...
    void ProcessMouseMovement(float xoffset, float yoffset)
    {
      xoffset *= MouseSensitivity;
      yoffset *= MouseSensitivity;

      Yaw += xoffset;
      Pitch += yoffset;

      misc_clamp(&Pitch, -89.0f, 89.9f);

      updateCameraVectors();
    }

    // ...
    void ProcessMouseScroll(float yoffset)
    {
      Zoom -= yoffset;

      misc_clamp(&Zoom, 0.01f, 90.0f);
    }

  // ...
  private:

    // ------------------------------------------------------------------------
    void updateCameraVectors()
    {
      // Calculate the new Front vector
      glm::vec3 front;
      front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
      front.y = sin(glm::radians(Pitch));
      front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
      
      // ...
      Front = glm::normalize(front);

      // Also re-calculate the Right and Up vector. Normalize the vectors, because 
      // their length gets closer to 0 the more you look up or down which results 
      // in slower movement.
      Right = glm::normalize(glm::cross(Front, WorldUp));
      Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif