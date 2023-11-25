#include <glm/glm.hpp>
#include "object.h"
#include "material.h"
#include "intersect.h"
#include <SDL2/SDL.h> // Asegúrate de incluir SDL para trabajar con texturas

struct NormalTx {
    glm::vec3 normal;
    glm::vec2 textureCoord;
    bool isTop = false;
    NormalTx(){};
    NormalTx(glm::vec3 normaled, glm::vec2 text, bool toped){
      normal = normaled;
      textureCoord = text;
      isTop = toped;
    };
};

class Cube : public Object {
public:
  Cube(const glm::vec3& center, float scale, const Material& mat);

  Intersect rayIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const override;

  void setTexture(SDL_Texture* tex) {
        texture = tex;
    }

private:
  glm::vec3 center;
  float scale;
  glm::vec3 minPoint;
  glm::vec3 maxPoint;
  NormalTx calculateNormal(const glm::vec3& point, const glm::vec3& minBounds, const glm::vec3& maxBounds, const glm::vec3& rayDirection) const;
  glm::vec2 calculateTexture(const glm::vec3& point, const glm::vec3& normal, const glm::vec3& minBounds) const;

  // Nuevas variables para texturas
  SDL_Texture* texture;
  glm::vec2 texCoords1[6]; // Coordenadas de textura para triángulo 1 de cada cara
  glm::vec2 texCoords2[6]; // Coordenadas de textura para triángulo 2 de cada cara
};
