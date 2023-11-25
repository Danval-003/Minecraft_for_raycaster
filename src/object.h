#pragma once

#include <glm/glm.hpp>
#include "material.h"
#include "intersect.h"

class Object {
public:
  Object(const Material& mat) : material(mat) {}
  virtual Intersect rayIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const = 0;
  
  virtual glm::vec2 getTextureCoordinates(const glm::vec3& point) const {
        // Por defecto, devuelve coordenadas de textura nulas
        return glm::vec2(0.0f, 0.0f);
    }

  Material material;
};
