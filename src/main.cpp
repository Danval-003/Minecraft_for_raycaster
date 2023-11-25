#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <cstdlib>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <print.h>
#include <glm/gtc/quaternion.hpp>

#include "color.h"
#include "intersect.h"
#include "object.h"
#include "sphere.h"
#include "cube.h"
#include "light.h"
#include "camera.h"
#include "imageloader.h"
#include "skybox.h"
#include "material.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float ASPECT_RATIO = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
const int MAX_RECURSION = 3;
const float BIAS = 0.0003f;
Skybox skybox("assets/dft.jpg");

SDL_Renderer* renderer;
std::vector<Object*> objects;
Light light(glm::vec3(-14, 20, 6.4), 1.5f, Color(255, 255, 255), 5.0f);
Camera camera(glm::vec3(0.0, 3.0, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f), 10.0f);


void point(glm::vec2 position, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(renderer, position.x, position.y);
}

float castShadow(const glm::vec3& shadowOrigin, const glm::vec3& lightDir, Object* hitObject) {
    float shadowIntensity = 1.0f;
    for (auto& obj : objects) {
        if (obj != hitObject) {
            Intersect shadowIntersect = obj->rayIntersect(shadowOrigin, lightDir);
            if (shadowIntersect.isIntersecting && shadowIntersect.dist > 0) {
                float shadowRatio = shadowIntersect.dist / glm::length(light.position - shadowOrigin);
                shadowRatio = glm::min(1.0f, shadowRatio);
                shadowIntensity *= shadowRatio;
            }
        }
    }
    return shadowIntensity;
}

void drawSmallEllipse(glm::vec3 center, int radiusX, int radiusY, Material mat) {
    for (int y = -radiusY; y <= radiusY; ++y) {
        for (int x = -radiusX; x <= radiusX; ++x) {
            // Verifica si el punto (x, y) está dentro de la elipse
            if (std::pow(x / static_cast<float>(radiusX), 2) + std::pow(y / static_cast<float>(radiusY), 2) <= 1.0) {
                glm::vec3 position(center);
                position.x += x;
                position.z += y;
                objects.push_back(new Cube(position, 1.0f, mat));
            }
        }
    }
}


Color castRay(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const short recursion = 0) {
    float zBuffer = 99999;
    Object* hitObject = nullptr;
    Intersect intersect;

    for (const auto& object : objects) {
        Intersect i;
        if (dynamic_cast<Cube*>(object) != nullptr) {
            i = dynamic_cast<Cube*>(object)->rayIntersect(rayOrigin, rayDirection);
        } else if (dynamic_cast<Sphere*>(object) != nullptr) {
            i = dynamic_cast<Sphere*>(object)->rayIntersect(rayOrigin, rayDirection);
        }
        if (i.isIntersecting && i.dist < zBuffer) {
            zBuffer = i.dist;
            hitObject = object;
            intersect = i;
        }
    }

    if (!intersect.isIntersecting || recursion == MAX_RECURSION) {
        return skybox.getColor(rayDirection);  // Sky color
    }

    glm::vec3 lightDir = glm::normalize(light.position - intersect.point);
    glm::vec3 viewDir = glm::normalize(intersect.point - rayOrigin);
    glm::vec3 reflectDir = glm::reflect(viewDir, intersect.normal);

    float shadowIntensity = castShadow(intersect.point, lightDir, hitObject);

    float diffuseLightIntensity = std::max(0.0f, glm::dot(intersect.normal, lightDir));
    float specReflection = glm::dot(viewDir, reflectDir);

    Material mat = hitObject->material;

    float specLightIntensity = std::pow(std::max(0.0f, glm::dot(viewDir, reflectDir)), mat.specularCoefficient);

    Color texColor = ImageLoader::getPixelColor(intersect.normal.y > 0 ? mat.topKey : mat.key, intersect.texture.x * mat.tSize.x, hitObject->material.tSize.y - intersect.texture.y * hitObject->material.tSize.y) * 0.6f;

    Color reflectedColor(0.0f, 0.0f, 0.0f);
    if (mat.reflectivity > 0) {
        glm::vec3 origin = intersect.point + intersect.normal * BIAS;
        reflectedColor = castRay(origin, reflectDir, recursion + 1);
    }

    Color refractedColor(0.0f, 0.0f, 0.0f);
    if (mat.transparency > 0) {
        glm::vec3 origin = intersect.point - intersect.normal * BIAS;
        glm::vec3 refractDir = glm::refract(rayDirection, intersect.normal, mat.refractionIndex);
        refractedColor = castRay(intersect.point, refractDir, recursion + 1);
    }

    Color diffuseLight = texColor * light.intensity * diffuseLightIntensity * mat.albedo * shadowIntensity;
    Color specularLight = light.color * light.intensity * specLightIntensity * mat.specularAlbedo * shadowIntensity;
    Color color = (diffuseLight + specularLight) * (1.0f - mat.reflectivity - mat.transparency) + reflectedColor * mat.reflectivity + refractedColor * mat.transparency;
    return color;
}


void setUp() {


    //Base de Esmeralda
    objects.push_back(new Cube({3,-2,3}, 1.0f, emerald));
    objects.push_back(new Cube({3,-2,2}, 1.0f, emerald));
    objects.push_back(new Cube({3,-2,1}, 1.0f, emerald));
    objects.push_back(new Cube({3,-2,0}, 1.0f, emerald));    objects.push_back(new Cube({2,-2,3}, 1.0f, emerald));    objects.push_back(new Cube({1,-2,3}, 1.0f, emerald));
    objects.push_back(new Cube({0,-2,3}, 1.0f, emerald));

    objects.push_back(new Cube({-3,-2,-3}, 1.0f, emerald));
    objects.push_back(new Cube({-3,-2,-2}, 1.0f, emerald));
    objects.push_back(new Cube({-3,-2,-1}, 1.0f, emerald));
    objects.push_back(new Cube({-3,-2,0}, 1.0f, emerald));    objects.push_back(new Cube({-2,-2,-3}, 1.0f, emerald));    objects.push_back(new Cube({-1,-2,-3}, 1.0f, emerald));
    objects.push_back(new Cube({0,-2,-3}, 1.0f, emerald));

    objects.push_back(new Cube({-3,-2,3}, 1.0f, emerald));
    objects.push_back(new Cube({-3,-2,2}, 1.0f, emerald));
    objects.push_back(new Cube({-3,-2,1}, 1.0f, emerald));   objects.push_back(new Cube({3,-2,-3}, 1.0f, emerald));    objects.push_back(new Cube({2,-2,-3}, 1.0f, emerald));    objects.push_back(new Cube({1,-2,-3}, 1.0f, emerald));

    objects.push_back(new Cube({3,-2,-2}, 1.0f, emerald));
    objects.push_back(new Cube({3,-2,-1}, 1.0f, emerald));    objects.push_back(new Cube({-2,-2,3}, 1.0f, emerald));    objects.push_back(new Cube({-1,-2,3}, 1.0f, emerald));

    //Base de oro
    objects.push_back(new Cube({2,-1,2}, 1.0f, gold));
    objects.push_back(new Cube({2,-1,1}, 1.0f, gold));
    objects.push_back(new Cube({2,-1,0}, 1.0f, gold));    objects.push_back(new Cube({1,-1,2}, 1.0f, gold));
    objects.push_back(new Cube({0,-1,2}, 1.0f, gold));

    objects.push_back(new Cube({-2,-1,-2}, 1.0f, gold));
    objects.push_back(new Cube({-2,-1,-1}, 1.0f, gold));
    objects.push_back(new Cube({-2,-1,0}, 1.0f, gold));    objects.push_back(new Cube({-1,-1,-2}, 1.0f, gold));
    objects.push_back(new Cube({0,-1,-2}, 1.0f, gold));

    objects.push_back(new Cube({2,-1,-2}, 1.0f, gold));
    objects.push_back(new Cube({2,-1,-1}, 1.0f, gold));   objects.push_back(new Cube({1,-1,-2}, 1.0f, gold));

    objects.push_back(new Cube({-2,-1,2}, 1.0f, gold));
    objects.push_back(new Cube({-2,-1,1}, 1.0f, gold));   objects.push_back(new Cube({-1,-1,2}, 1.0f, gold));


    //Base de diamante
    objects.push_back(new Cube({1,0,1}, 1.0f, diamond));
    objects.push_back(new Cube({1,0,0}, 1.0f, diamond));
    objects.push_back(new Cube({0,0,1}, 1.0f, diamond));

    objects.push_back(new Cube({-1,0,-1}, 1.0f, diamond));
    objects.push_back(new Cube({-1,0,0}, 1.0f, diamond));
    objects.push_back(new Cube({0,0,-1}, 1.0f, diamond));

    objects.push_back(new Cube({1,0,-1}, 1.0f, diamond));
    objects.push_back(new Cube({-1,0,1}, 1.0f, diamond));
    objects.push_back(new Cube({0,0,0}, 1.0f, diamond));

    objects.push_back(new Cube({0,1,0}, 1.0f, beacon));
    objects.push_back(new Cube({0,1.5,0}, 0.5f, lig));
    objects.push_back(new Cube({0,2,0}, 0.5f, lig));
    objects.push_back(new Cube({0,2.5,0}, 0.5f, lig));
    objects.push_back(new Cube({0,3,0}, 0.5f, lig));

}

void render() {
    float fov = 3.1415/3;
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {

            float random_value = static_cast<float>(std::rand())/static_cast<float>(RAND_MAX);
            if (random_value < 0.0) {
                continue;
            }



            float screenX = (2.0f * (x + 0.5f)) / SCREEN_WIDTH - 1.0f;
            float screenY = -(2.0f * (y + 0.5f)) / SCREEN_HEIGHT + 1.0f;
            screenX *= ASPECT_RATIO;
            screenX *= tan(fov/2.0f);
            screenY *= tan(fov/2.0f);


            glm::vec3 cameraDir = glm::normalize(camera.target - camera.position);

            glm::vec3 cameraX = glm::normalize(glm::cross(cameraDir, camera.up));
            glm::vec3 cameraY = glm::normalize(glm::cross(cameraX, cameraDir));
            glm::vec3 rayDirection = glm::normalize(
                cameraDir + cameraX * screenX + cameraY * screenY
            );

            Color pixelColor = castRay(camera.position, rayDirection);
            /* Color pixelColor = castRay(glm::vec3(0,0,20), glm::normalize(glm::vec3(screenX, screenY, -1.0f))); */

            point(glm::vec2(x, y), pixelColor);
        }
    }
}

int main(int argc, char* argv[]) {
    ImageLoader::loadImage("oak_plank", "assets/oak_planks.png");
    ImageLoader::loadImage("stone", "assets/stone.png");
    ImageLoader::loadImage("dirt", "assets/dirt.png");
    ImageLoader::loadImage("granite", "assets/granite.png");
    ImageLoader::loadImage("diorite", "assets/diorite.png");
    ImageLoader::loadImage("coal_ore", "assets/coal_ore.png");
    ImageLoader::loadImage("andesite", "assets/andesite.png");
    ImageLoader::loadImage("oak_planks_rail", "assets/oak_planks_rail.png");
    ImageLoader::loadImage("stone_rail", "assets/stone_rail.png");
    ImageLoader::loadImage("dirt_rail", "assets/dirt_rail.png");
    ImageLoader::loadImage("dirt_rail2", "assets/dirt_rail2.png");
    ImageLoader::loadImage("tnt", "assets/tnt_side.png");
    ImageLoader::loadImage("tnt_top", "assets/tnt_top.png");
    ImageLoader::loadImage("water", "assets/pack.png");
    ImageLoader::loadImage("grass", "assets/grass_block_side.png");
    ImageLoader::loadImage("grassTop", "assets/grass_b_top.png");
    ImageLoader::loadImage("glass", "assets/glass_n.png");
    ImageLoader::loadImage("diamond", "assets/diamond_block.png");
    ImageLoader::loadImage("gold", "assets/gold_block.png");
    ImageLoader::loadImage("iron", "assets/iron_block.png");
    ImageLoader::loadImage("emerald", "assets/emerald_block.png");
    ImageLoader::loadImage("beacon", "assets/beacon.png");
    ImageLoader::loadImage("light", "assets/white_concrete.png");

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Hello World - FPS: 0", 
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                          SCREEN_WIDTH, SCREEN_HEIGHT, 
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    camera.rotate(5.0f, 0.0f);

    bool running = true;
    SDL_Event event;

    int frameCount = 0;
    Uint32 startTime = SDL_GetTicks();
    Uint32 currentTime = startTime;
    
    setUp();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        camera.move(-1.0f);
                        break;
                    case SDLK_DOWN:
                        camera.move(1.0f);
                        break;
                    case SDLK_LEFT:
                        print("left");
                        camera.rotate(-1.0f, 0.0f);
                        break;
                    case SDLK_RIGHT:
                        print("right");
                        camera.rotate(1.0f, 0.0f);
                        break;
                    case SDLK_w:
                        camera.upper(1.0f);
                        break;
                    case SDLK_s:
                        camera.upper(-1.0f);
                        break;
                    case SDLK_i:
                        light.position.y+=1.0f;
                        std::cout<<light.position.y<<std::endl;
                        break;
                    case SDLK_k:
                        light.position.y-=1.0f;
                        std::cout<<light.position.y<<std::endl;
                        break;
                    case SDLK_j:
                        light.rotate(-1.0f,0.0f);
                        std::cout<<light.position.x<<" "<<light.position.z<<" "<<light.position.y<<std::endl;
                        break;
                    case SDLK_l:
                        light.rotate(1.0f,0.0f);
                        std::cout<<light.position.x<<" "<<light.position.z<<" "<<light.position.y<<std::endl;
                        break;
                 }
            }


        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        render();

        // Present the renderer
        SDL_RenderPresent(renderer);

        frameCount++;

        // Calculate and display FPS
        if (SDL_GetTicks() - currentTime >= 1000) {
            currentTime = SDL_GetTicks();
            std::string title = "Hello World - FPS: " + std::to_string(frameCount);
            SDL_SetWindowTitle(window, title.c_str());
            frameCount = 0;
        }
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

