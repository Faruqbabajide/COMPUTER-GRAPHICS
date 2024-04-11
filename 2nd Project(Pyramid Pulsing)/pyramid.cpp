#include "pyramid.h"
#include "Renderer3D.h"
#include <iostream>
#include <vector>
#include <SDL.h>

//Creates an array of points
std::vector<Point3D> points{
        Point3D{ 0, 1, 0 },  // Top point
        Point3D{ -1, -1, 1 }, // Base vertices
        Point3D{ 1, -1, 1 },
        Point3D{ 1, -1, -1 },
        Point3D{ -1, -1, -1 }
};

//creates an array of edges (This hold indices referencing to the points array)
std::vector<Edge> edges{
            Edge{ 0, 1 },
        Edge{ 0, 2 },
        Edge{ 0, 3 },
        Edge{ 0, 4 },
        Edge{ 1, 2 },
        Edge{ 2, 3 },
        Edge{ 3, 4 },
        Edge{ 4, 1 }
};

//default C++ function
int main(int argc, char* argv[])
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window and renderer
    SDL_Window* window = SDL_CreateWindow("Pyramid Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create an instance of PyramidRenderer
    PyramidRenderer pyramidRenderer(window, renderer, points, edges);

    bool running = true;
    while (running)
    {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // Render the pyramid using PyramidRenderer
        pyramidRenderer.render();
    }

    // Cleanup and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}