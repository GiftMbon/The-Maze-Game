3D Maze Game
Overview

The 3D Maze Game is a simple 3D rendering engine built using SDL2, allowing players to navigate through a maze. The game employs raycasting to render the 3D environment, providing a retro-style gaming experience reminiscent of classic first-person shooters.

Features

3D Maze Rendering: Utilizes raycasting to project a 3D view of the maze based on player position and direction.
Player Movement: Move the player using keyboard controls (WASD) to navigate through the maze.
Textured Walls: Walls are textured to provide a more immersive visual experience.

Requirements

C++ compiler
SDL2 library
BMP image files for textures


Install SDL2: Follow the installation instructions for SDL2 appropriate for your operating system.

#Prepare Textures:   
Place the following BMP texture files in the project directory:

wall_texture.bmp
floor_texture.bmp
ceiling_texture.bmp
Ensure the textures are 64x64 pixels.

#Compilation
Compile the project using g++ with SDL2 linked:  
g++ -Isrc/include -Lsrc/lib -o maze maze.cpp -lmingw32 -lSDL2main -lSDL2

#Running the Game
Run the compiled program:
./maze.exe


#Controls
W: Move forward  
S: Move backward  
A: Rotate left  
D: Rotate right  
ESC: Exit the game
