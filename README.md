# Raycasting Minecraft Diorama

This project demonstrates a simple raycasting engine to render a Minecraft-themed diorama. The scene includes blocks representing different materials such as light, gold, diamond, emerald, and beacon. Each material has specific optical properties like reflection and refraction.

![Imagen](https://github.com/Danval-003/Minecraft_for_raycaster/blob/main/forReadme/videoRead.gif?raw=true)

![Alternativo](https://github.com/Danval-003/Minecraft_for_raycaster/blob/main/forReadme/alternativo.gif?raw=true)

## Scene Overview
The scene consists of a 3D diorama constructed using Minecraft-style blocks. There are five materials in the scene:

1. **Light (Beacon):** Transparent material with reflection and refraction.
2. **Gold:** Mostly reflective.
3. **Diamond:** Reflective with a minimal amount of refraction.
4. **Emerald:** Reflective material with slight reflection.
5. **Beacon:** Partially refractive material.

## Video Explanation
[Watch the video explanation of the scene](https://youtu.be/Vh-6eoR7GNo)

## Prerequisites
- C++ compiler with support for C++11
- SDL2 library for window management and rendering

## Compilation and Execution
Make sure you have SDL2 installed. You can compile the project using the following commands:

```bash
g++ -std=c++11 -o raycasting main.cpp -lSDL2 -lSDL2_image
./raycasting
```

## Controls
- Arrow keys: Move the camera.
- WASD keys: Adjust the camera's orientation.
- I/K keys: Raise/lower the light source.
- J/L keys: Rotate the light source.

## Code Structure
The code is organized into several files for better readability and maintainability. Key components include:
- `color.h`: Definition of the `Color` class.
- `intersect.h`: Definition of the `Intersect` class for ray-object intersection.
- `object.h`, `sphere.h`, `cube.h`: Classes representing 3D objects and their specific implementations.
- `light.h`: Definition of the `Light` class.
- `camera.h`: Definition of the `Camera` class for handling the viewpoint.
- `imageloader.h`: Utility for loading images/textures.
- `skybox.h`: Implementation of a skybox.
- `material.h`: Definition of the `Material` class.

## Rendering and Shadow Casting
The `castRay` function traces rays through the scene and calculates the color of each pixel. The `castShadow` function determines the shadow intensity at a point based on the light source's visibility.

## Scene Setup
The `setUp` function initializes the scene by placing cubes representing different materials in specific configurations.

## Acknowledgments
- This project is inspired by Minecraft and uses SDL2 for rendering.

Feel free to explore and modify the code to create your own Minecraft-themed scenes or experiment with raycasting techniques. If you have any questions or suggestions, please refer to the [video explanation](https://youtu.be/Vh-6eoR7GNo) or contact the author.

Happy coding!
