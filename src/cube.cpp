#include <glm/detail/qualifier.hpp>
#include <glm/glm.hpp>
#include "object.h"
#include "material.h"
#include "intersect.h"
#include "cube.h"



Cube::Cube(const glm::vec3& center, float scale, const Material& mat)
  : center(center), scale(scale), Object(mat) {
  // Calculate minPoint and maxPoint based on the center and scale
  minPoint = center - 0.5f * scale;
  maxPoint = center + 0.5f * scale;
}

Intersect Cube::rayIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const {
  glm::vec3 halfExtents = glm::vec3(scale) * 0.5f;
  glm::vec3 minBounds = center - halfExtents;
  glm::vec3 maxBounds = center + halfExtents;
  glm::vec3 tMin = (minPoint - rayOrigin) / rayDirection;
  glm::vec3 tMax = (maxPoint - rayOrigin) / rayDirection;

  glm::vec3 t1 = glm::min(tMin, tMax);
  glm::vec3 t2 = glm::max(tMin, tMax);

  float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
  float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);

  if (tNear > tFar || tFar < 0) {
    return Intersect{false};
  }

  float dist = (tNear < 0.0f) ? tFar : tNear;

  glm::vec3 hitPoint = rayOrigin + dist * rayDirection;
  NormalTx n = calculateNormal(hitPoint, minBounds, maxBounds, rayDirection);



  return Intersect{true, tNear, hitPoint, n.normal, n.textureCoord, n.isTop};
}

NormalTx Cube::calculateNormal(const glm::vec3& point, const glm::vec3& minBounds, const glm::vec3& maxBounds, const glm::vec3& rayDirection) const {
  glm::vec3 normal(0.0f);
  float epsilon = 0.001f;
  bool top = false;

  if (fabs(point.x - minBounds.x) < epsilon) normal.x = -1.0f;
  if (fabs(point.x - maxBounds.x) < epsilon) normal.x = 1.0f;
  if (fabs(point.y - minBounds.y) < epsilon) normal.y = -1.0f;
  if (fabs(point.y - maxBounds.y) < epsilon) normal.y = 1.0f;
  if (fabs(point.z - minBounds.z) < epsilon) normal.z = -1.0f;
  if (fabs(point.z - maxBounds.z) < epsilon) normal.z = 1.0f;

  if(normal.y >0) top = true;
  glm::vec2 textureCoord = calculateTexture(point,normal ,minBounds );

  if (glm::dot(rayDirection, normal) > 0) {
    normal = -normal;
  }


  return NormalTx{normal, textureCoord, top};
}


glm::vec2 Cube::calculateTexture(const glm::vec3& point, const glm::vec3& normal, const glm::vec3& minBounds) const{
    glm::vec3 hitVector = point - minBounds;

    glm::vec2 texCoord(0.0f);

    if (normal.x != 0) {
      texCoord.x = hitVector.z / scale;
      texCoord.y = hitVector.y / scale;
    }
    else if (normal.y != 0) {
      texCoord.x = hitVector.x / scale;
      texCoord.y = hitVector.z / scale;
    }
    else if (normal.z != 0)
    {
      texCoord.x = hitVector.x / scale;
      texCoord.y = hitVector.y / scale;
    }

    if (normal.x < 0 || normal.y < 0 || normal.z < 0)
    {
      texCoord = glm::vec2(1.0f) - texCoord;
    }

    return texCoord;
}


