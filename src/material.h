#pragma once

#include "color.h"
#include <string.h>
#include <glm/glm.hpp>

struct Material {
  Color diffuse;
  float albedo;
  float specularAlbedo;
  float specularCoefficient;
  float reflectivity;
  float transparency;
  float refractionIndex;
  std::string key;
  std::string topKey;
  glm::vec2 tSize{128, 128};
};

extern Material water;
extern Material stone;
extern Material dirt;
extern Material lamp;
extern Material grass;
extern Material wood;
extern Material glass;
extern Material diorite;
extern Material diamond;
extern Material emerald;
extern Material iron;
extern Material gold;
extern Material beacon;
extern Material lig;
