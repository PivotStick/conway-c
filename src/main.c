#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

const int CELL_SIZE = 15;
const int WIDTH = 50;
const int HEIGHT = 50;

int getAliveNeighboors(bool grid[WIDTH][HEIGHT], int x, int y)
{
    int xOff;
    int yOff;

    int aliveCount = 0;

    for (xOff = -1; xOff <= 1; xOff++)
        for (yOff = -1; yOff <= 1; yOff++)
        {
            int xI = x + xOff;
            int yI = y + yOff;

            if (xI < 0 || yI < 0 || xI > WIDTH -1 || yI > HEIGHT - 1)
                continue;

            if (xOff == 0 && yOff == 0)
                continue;

            if (grid[xI][yI])
                aliveCount++;
        }

    return aliveCount;
}

int main()
{
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;

    bool cells[WIDTH][HEIGHT];
    bool tmpCells[WIDTH][HEIGHT];

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
        SDL_WINDOW_SHOWN
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

    for (int x = 0; x < WIDTH; x++)
        for (int y = 0; y < HEIGHT; y++)
            cells[x][y] = false;

    SDL_Rect rect;
    rect.h = CELL_SIZE;
    rect.w = CELL_SIZE;
    rect.x = 0;
    rect.y = 0;

    bool running = true;
    bool playing = false;

    int mouseX = 0;
    int mouseY = 0;

    int xIndex;
    int yIndex;

    while (running)
    {
        if (SDL_PollEvent(&event))
            switch (event.type)
            {
            case SDL_QUIT: running = false; break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&mouseX, &mouseY);

                xIndex = mouseX / CELL_SIZE;
                yIndex = mouseY / CELL_SIZE;

                bool *cell = &cells[xIndex][yIndex];
                *cell = !*cell;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_SPACE)
                    playing = !playing;

                break;
            default: break;
            }

        // Logic
        if (playing)
        {
            for (int x = 0; x < WIDTH; x++)
                for (int y = 0; y < HEIGHT; y++)
                    tmpCells[x][y] = cells[x][y];
            
            for (int x = 0; x < WIDTH; x++)
                for (int y = 0; y < HEIGHT; y++)
                {
                    int aliveCount = getAliveNeighboors(tmpCells, x, y);
                    bool *cell = &cells[x][y];

                    if (aliveCount == 3)
                        *cell = true;
                    else if (aliveCount < 2 || aliveCount > 3)
                        *cell = false;
                }
        }

        // Display
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        for (int x = 0; x < WIDTH; x++)
            for (int y = 0; y < HEIGHT; y++)
            {
                int color = cells[x][y] ? 0 : 255;

                rect.x = x * rect.w;
                rect.y = y * rect.h;

                SDL_SetRenderDrawColor(renderer, color, color, color, 0);
                SDL_RenderFillRect(renderer, &rect);
            }
        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}