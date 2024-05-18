#include "test.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IMAGE_WIDTH = SCREEN_WIDTH + 500;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
int textureWidthDiff = IMAGE_WIDTH - SCREEN_WIDTH;

SDL_Texture* loadTexture(const std::string& path) {
    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

bool initSDL()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    gWindow = SDL_CreateWindow("2D Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Set renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Allows for (Initializes) Image Loading abilities using SDL_image library
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void closeSDL()
{
    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

    std::vector<SDL_Texture*> backgroundTextures;
    backgroundTextures.push_back(loadTexture("./Assets/Pixel_Forest/layer0.jpg"));
    backgroundTextures.push_back(loadTexture("./Assets/Pixel_Forest/layer1(1)(1).png"));
    backgroundTextures.push_back(loadTexture("./Assets/Pixel_Forest/layer1(1).png"));

    if (backgroundTextures[0] == NULL || backgroundTextures[1] == NULL || backgroundTextures[2] == NULL) {
        printf("Failed to load textures!\n");
        return -1;
    }

    // Define ball's position and velocity
    float ballX = 0.0f;
    float ballY = 300.0f;
    float ballVelX = 2.0f;
    float ballVelY = 2.0f;

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Update ball position
        ballX += ballVelX;
        ballY += ballVelY;

        // Bounce off edges
        if (ballX < 0 || ballX + IMAGE_WIDTH * 0.5 > SCREEN_WIDTH) {
            ballVelX = -ballVelX;
        }
        if (ballY < 0 || ballY + SCREEN_HEIGHT * 0.5 > SCREEN_HEIGHT) {
            ballVelY = -ballVelY;
        }

        // Clear screen
        SDL_RenderClear(gRenderer);
        float scaleFactor = 0.5;
        float scalesFactor = 0.8;
        float scalessFactor = 0.2;

        // Render the first image
        SDL_Rect textureDest0 = { 0, 0, IMAGE_WIDTH * scalesFactor , SCREEN_HEIGHT * scalesFactor };
        SDL_RenderCopy(gRenderer, backgroundTextures[0], nullptr, &textureDest0);
G
        // Render the second image
        SDL_Rect textureDest1 = { 0, 240, IMAGE_WIDTH * scalesFactor , SCREEN_HEIGHT * scalesFactor }; // Example position
        SDL_RenderCopy(gRenderer, backgroundTextures[1], nullptr, &textureDest1);

        // Render the moving ball (third image)
        SDL_Rect textureDest2 = { (int)ballX, (int)ballY, IMAGE_WIDTH * scalessFactor, SCREEN_HEIGHT * scalessFactor }; // Example position
        SDL_RenderCopy(gRenderer, backgroundTextures[2], nullptr, &textureDest2);

        // Update screen
        SDL_RenderPresent(gRenderer);
    }

    // Destroy textures
    for (SDL_Texture* texture : backgroundTextures) {
        SDL_DestroyTexture(texture);
    }

    // Close SDL
    closeSDL();

    return 0;
}
