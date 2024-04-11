#include "Renderer3D.h"
#include <SDL.h>

PyramidRenderer::PyramidRenderer(SDL_Window* window, SDL_Renderer* renderer, std::vector<Point3D>& _points, std::vector<Edge>& _edges)
{
    SDL_GetWindowSize(window, &WindowSizeX, &WindowSizeY);
    SDL_render = renderer;
    points = _points;
    edges = _edges;
}

void PyramidRenderer::render()
{
    auto time1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration(0);

    SDL_SetRenderDrawColor(SDL_render, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(SDL_render);
    SDL_SetRenderDrawColor(SDL_render, 0, 0, 203, SDL_ALPHA_OPAQUE);

    calculateScale();

    for (auto& edge : edges) {
        Point2D start = projection(Point3D{ points[edge.start].x * scale, points[edge.start].y * scale, points[edge.start].z * scale });
        Point2D end = projection(Point3D{ points[edge.end].x * scale, points[edge.end].y * scale, points[edge.end].z * scale });

        SDL_RenderDrawLine(SDL_render, start.x, start.y, end.x, end.y);
    }

    SDL_RenderPresent(SDL_render);

    auto time2 = std::chrono::high_resolution_clock::now();
    duration = time2 - time1;
    DeltaTime = duration.count();
    time1 = time2;
}

void PyramidRenderer::calculateScale()
{
    // Calculate scale based on time
    scale = 1.0f + 0.1f * sin(SDL_GetTicks() * 0.005f); // Adjust the frequency and magnitude as needed
}

Point2D PyramidRenderer::projection(Point3D point)
{
    return Point2D{ WindowSizeX / 2 + (point.x * FL) / (FL + point.z) * 100, WindowSizeY / 2 + (point.y * FL) / (FL + point.z) * 100 };
}