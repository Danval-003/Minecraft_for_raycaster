#include "material.h"


Material stone = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "stone",
        "stone",
        glm::vec2(128, 128),
};


Material water = {
        Color(43, 60, 244),   // diffuse
        1.0,
        0.3,
        10.0f,
        0.15f,
        0.2f,
        1.333f,
        "water",
        "water",
        glm::vec2(128, 128),
};

Material dirt = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "dirt",
        "dirt",
        glm::vec2(128, 128),
};


Material lamp = {
        Color(43, 60, 244),   // diffuse
        1.0,
        0.3,
        10.0f,
        0.0f,
        0.5f,
        0.5f,
        "lamp",
        "lamp",
        glm::vec2(128, 128),
};


Material wood = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "oak_plank",
        "oak_plank",
        glm::vec2(128, 128),
};


Material grass = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "grass",
        "grassTop",
        glm::vec2(128, 128),
};


Material glass = {
        Color(43, 60, 244),   // diffuse
        1.0,
        0.3,
        10.0f,
        0.0f,
        0.9f,
        1.5f,
        "glass",
        "glass",
        glm::vec2(128, 128),
};

Material diorite = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "diorite",
        "diorite",
        glm::vec2(128, 128),
    };



Material gold = {
        Color(255, 255, 255),   // diffuse
        1.0,
        0.9,
        1455.0f,
        0.25f,
        0.0f,
        0.0f,
        "gold",
        "gold",
        glm::vec2(128, 128),
    };

Material diamond = {
        Color(255, 255, 255),   // diffuse
        1.0,
        0.9,
        10.0f,
        0.2f,
        0.3f,
        1.5f,
        "diamond",
        "diamond",
        glm::vec2(128, 128),
    };

Material iron = {
        Color(255, 255, 255),   // diffuse
        0.5,
        0.9,
        1.0f,
        0.05f,
        0.0f,
        0.0f,
        "iron",
        "iron",
        glm::vec2(128, 128),
    };

Material emerald = {
        Color(255, 255, 255),   // diffuse
        0.5,
        0.9,
        1.0f,
        0.05f,
        0.0f,
        0.0f,
        "emerald",
        "emerald",
        glm::vec2(128, 128),
    };

Material beacon = {
        Color(255, 255, 255),   // diffuse
        1.0,
        0.3,
        2000.0f,
        0.0f,
        0.7f,
        1.5f,
        "beacon",
        "beacon",
        glm::vec2(128, 128),
};


Material lig = {
        Color(255,255,255),   // diffuse
        1.0,
        1450.3,
        10.0f,
        0.0f,
        0.9f,
        1.3f,
        "light",
        "light",
        glm::vec2(128, 128),
};
