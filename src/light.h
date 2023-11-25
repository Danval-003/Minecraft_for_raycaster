#pragma once

#include <glm/glm.hpp>
#include "color.h"

class Light {
public:
  glm::vec3 position;
  float intensity;
  Color color;

  float rotationSpeed;

  Light(glm::vec3 position, float intensity, Color color, float rotationSpeed);

  void rotate(float deltaX, float deltaY);

  void move(float deltaZ);

  void upper(float de);
};
