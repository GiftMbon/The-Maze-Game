#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Maze dimensions
const int MAP_WIDTH = 16;
const int MAP_HEIGHT = 16;
const int TILE_SIZE = 50;  // Adjusted wall size

// Player setup
struct Player {
    double x, y;      // Player position
    double dirX, dirY; // Player direction
    double planeX, planeY; // Camera plane (for FOV)
    double moveSpeed, rotSpeed;
};

// Simple maze layout (1 = wall, 0 = empty space)
int maze[MAP_WIDTH][MAP_HEIGHT] = {
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// Function prototypes
void render3DMaze(SDL_Renderer* renderer, SDL_Texture* wallTexture, SDL_Texture* floorTexture, SDL_Texture* ceilingTexture, Player& player);

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("3D Maze Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Load textures
    SDL_Surface* wallSurface = SDL_LoadBMP("wall_texture.bmp");
    SDL_Texture* wallTexture = SDL_CreateTextureFromSurface(renderer, wallSurface);
    SDL_FreeSurface(wallSurface);

    SDL_Surface* floorSurface = SDL_LoadBMP("floor_texture.bmp");
    SDL_Texture* floorTexture = SDL_CreateTextureFromSurface(renderer, floorSurface);
    SDL_FreeSurface(floorSurface);

    SDL_Surface* ceilingSurface = SDL_LoadBMP("ceiling_texture.bmp");
    SDL_Texture* ceilingTexture = SDL_CreateTextureFromSurface(renderer, ceilingSurface);
    SDL_FreeSurface(ceilingSurface);

    if (!wallTexture || !floorTexture || !ceilingTexture) {
        std::cerr << "Failed to load textures! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize player
    Player player = {3.5, 3.5, -1, 0, 0, 0.66, 0.01, 0.01};  // Adjusted moveSpeed and rotSpeed

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Get keyboard state for movement
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_W]) {
            if (!maze[int(player.x + player.dirX * player.moveSpeed)][int(player.y)]) {
                player.x += player.dirX * player.moveSpeed;
            }
            if (!maze[int(player.x)][int(player.y + player.dirY * player.moveSpeed)]) {
                player.y += player.dirY * player.moveSpeed;
            }
        }
        if (keystate[SDL_SCANCODE_S]) {
            if (!maze[int(player.x - player.dirX * player.moveSpeed)][int(player.y)]) {
                player.x -= player.dirX * player.moveSpeed;
            }
            if (!maze[int(player.x)][int(player.y - player.dirY * player.moveSpeed)]) {
                player.y -= player.dirY * player.moveSpeed;
            }
        }
        if (keystate[SDL_SCANCODE_A]) {
            double oldDirX = player.dirX;
            player.dirX = player.dirX * cos(player.rotSpeed) - player.dirY * sin(player.rotSpeed);
            player.dirY = oldDirX * sin(player.rotSpeed) + player.dirY * cos(player.rotSpeed);
            double oldPlaneX = player.planeX;
            player.planeX = player.planeX * cos(player.rotSpeed) - player.planeY * sin(player.rotSpeed);
            player.planeY = oldPlaneX * sin(player.rotSpeed) + player.planeY * cos(player.rotSpeed);
        }
        if (keystate[SDL_SCANCODE_D]) {
            double oldDirX = player.dirX;
            player.dirX = player.dirX * cos(-player.rotSpeed) - player.dirY * sin(-player.rotSpeed);
            player.dirY = oldDirX * sin(-player.rotSpeed) + player.dirY * cos(-player.rotSpeed);
            double oldPlaneX = player.planeX;
            player.planeX = player.planeX * cos(-player.rotSpeed) - player.planeY * sin(-player.rotSpeed);
            player.planeY = oldPlaneX * sin(-player.rotSpeed) + player.planeY * cos(-player.rotSpeed);
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Render the 3D maze
        render3DMaze(renderer, wallTexture, floorTexture, ceilingTexture, player);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyTexture(wallTexture);
    SDL_DestroyTexture(floorTexture);
    SDL_DestroyTexture(ceilingTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void render3DMaze(SDL_Renderer* renderer, SDL_Texture* wallTexture, SDL_Texture* floorTexture, SDL_Texture* ceilingTexture, Player& player) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        // Ray position and direction
        double cameraX = 2 * x / double(SCREEN_WIDTH) - 1; // Camera plane goes from -1 to 1
        double rayDirX = player.dirX + player.planeX * cameraX;
        double rayDirY = player.dirY + player.planeY * cameraX;

        // Which box of the map we're in
        int mapX = int(player.x);
        int mapY = int(player.y);

        // Length of ray from one x or y-side to next x or y-side
        double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

        // Length of ray from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        // Direction to step in x or y (+1 or -1)
        int stepX;
        int stepY;

        // Calculate step and initial sideDist
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (player.x - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player.x) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (player.y - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player.y) * deltaDistY;
        }

        // Perform DDA (Digital Differential Analysis)
        int hit = 0; // Was there a wall hit?
        int side;    // Was it a NS or an EW wall?

        while (hit == 0) {
            // Jump to next map square, either in x-direction, or in y-direction
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            // Check if ray has hit a wall
            if (maze[mapX][mapY] > 0) hit = 1;
        }

        // Calculate distance projected on camera direction (perpendicular distance)
        double perpWallDist;
        if (side == 0)
            perpWallDist = (mapX - player.x + (1 - stepX) / 2) / rayDirX;
        else
            perpWallDist = (mapY - player.y + (1 - stepY) / 2) / rayDirY;

        // Calculate height of line to draw on screen
        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        // Calculate lowest and highest pixel to fill in the current stripe
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

        // Determine wall texture region
        SDL_Rect srcRect;
        srcRect.w = 64; // Width of the texture
        srcRect.h = 64; // Height of the texture
       
// Calculate the wall's texture coordinates
        double wallX; // Exact x coordinate of the wall intersection
        if (side == 0) {
            wallX = player.y + perpWallDist * rayDirY;
        } else {
            wallX = player.x + perpWallDist * rayDirX;
        }
        wallX -= floor(wallX);

        // Texture coordinate (0 to 1)
        int texX = int(wallX * double(srcRect.w));

        // Determine wall color based on side hit (NS vs EW) for shading
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White for the walls
        if (side == 1) SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Darker gray for EW walls

        // Draw the vertical stripe corresponding to the wall slice
        SDL_Rect destRect;
        destRect.x = x;                     // X position on the screen
        destRect.y = drawStart;            // Start Y position on the screen
        destRect.w = 1;                    // Width of the stripe
        destRect.h = drawEnd - drawStart;  // Height of the stripe

        // Render the wall texture
        SDL_Rect srcRectFinal;
        srcRectFinal.x = texX;             // X coordinate in the texture
        srcRectFinal.y = 0;               // Y coordinate in the texture
        srcRectFinal.w = 1;               // Width of the texture slice
        srcRectFinal.h = srcRect.h;       // Full height of the texture

        SDL_RenderCopy(renderer, wallTexture, &srcRectFinal, &destRect);
    }
     
    }