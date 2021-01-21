#include <stdio.h>
#include <stdbool.h>
#include "game.h"

void SDL_ExitError(const char *message)
{
    printf("%s > %s", message, SDL_GetError());
    exit(EXIT_FAILURE);
}

int main()
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        SDL_ExitError("Couldn't init SDL");
    
    window = SDL_CreateWindow(
        "Game Of Life",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL)
        SDL_ExitError("Couldn't create window");

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == NULL)
        SDL_ExitError("Couldn't create renderer");

    SDL_Event event;
    bool isRunning = true;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;

    double deltaTime = 0;

    Start(window);

    while (isRunning)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

        if (SDL_PollEvent(&event))
            switch (event.type)
            {
                case SDL_QUIT: isRunning = false; break;
                case SDL_MOUSEBUTTONDOWN: OnMouseDown(&event); break;
                case SDL_MOUSEBUTTONUP: OnMouseReleased(&event); break;
                case SDL_MOUSEMOTION: OnMouseMotion(&event); break;
                case SDL_KEYDOWN: OnKeyPressed(&event.key.keysym.sym); break;
                case SDL_KEYUP: OnKeyUp(&event.key.keysym.sym); break;
                default: break;
            }

        Update(&deltaTime);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        Draw(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}