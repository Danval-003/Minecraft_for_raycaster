#include "light.h"
#include "color.h"
#include <glm/gtc/quaternion.hpp>

Light::Light(glm::vec3 position, float intensity, Color color, float rotationSpeed)
  : position(position), intensity(intensity), color(color), rotationSpeed(rotationSpeed)
{}

void Light::rotate(float deltaX, float deltaY) {
  glm::quat quatRotY = glm::angleAxis(glm::radians(deltaX * rotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::quat quatRotX = glm::angleAxis(glm::radians(deltaY * rotationSpeed), glm::vec3(1.0f, 0.0f, 0.0f));

  position = quatRotY * (position);
  position = quatRotY * (position);
}

void Light::move(float deltaZ) {
  glm::vec3 dir = glm::normalize(position);
  position += dir * deltaZ;
}

void Light::upper(float de){
  position.y += de;
}
