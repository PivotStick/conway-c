#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

int main()
{
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;

    const int CELL_SIZE = 20;
    const int WIDTH = 20;
    const int HEIGHT = 20;

    bool cells[WIDTH][HEIGHT];

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Impossible d'initialiser la SDL\n");
        return -1;
    }

    window = SDL_CreateWindow(
        "Guillaume le 2",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH * CELL_SIZE,
        HEIGHT * CELL_SIZE,
        SDL_WINDOW_OPENGL
    );

    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (window == NULL)
    {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return -1;
    }

    int mouseX;
    int mouseY;

    for (int c = 0; c < WIDTH; c++)
        for (int l = 0; l < HEIGHT; l++)
            cells[c][l] = true;

    SDL_Rect rect;
    rect.h = CELL_SIZE;
    rect.w = CELL_SIZE;
    rect.x = 0;
    rect.y = 0;

    while (true)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        for (int c = 0; c < WIDTH; c++)
            for (int l = 0; l < HEIGHT; l++)
            {
                int isAlive = cells[c][l];
                if (isAlive)
                    isAlive = 255;

                SDL_SetRenderDrawColor(renderer, isAlive, isAlive, isAlive, 0);
                rect.x = c * CELL_SIZE;
                rect.y = l * CELL_SIZE;
                SDL_RenderDrawRect(renderer, &rect);
            }
        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}