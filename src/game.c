#include <stdbool.h>
#include "game.h"

const signed int CELLSIZE = 5;
const signed int WIDTH = 120 * 2;
const signed int HEIGHT = 60 * 2;

const int SIZE = WIDTH * HEIGHT;

bool isPlaying = false;
bool isDrawing = false;
bool shouldErase = false;

bool cells[SIZE] = { 0 };
bool oldCells[SIZE] = { 0 };
bool checkPoint[SIZE] = { 0 };

int mouseX, mouseY;

double timer = 0;
double frameRate = 0.05;

SDL_Rect rect = {
    0, 0,
    CELLSIZE, CELLSIZE
};

#pragma region Cells utils
int GetCellIndex(int x, int y)
{
    return x + (y * WIDTH);
}

void GetCellCoordinates(int i, int *x, int *y)
{
    *x = i % WIDTH;
    *y = i / WIDTH % HEIGHT;
}

void SetCell(int x, int y, bool value)
{
    cells[GetCellIndex(x, y)] = value;
}

bool GetCell(int x, int y)
{
    return cells[GetCellIndex(x, y)];
}

void SetAtMousePos()
{
    SDL_GetMouseState(&mouseX, &mouseY);

    SetCell(
        mouseX / CELLSIZE,
        mouseY / CELLSIZE,
        !shouldErase
    );
}

int GetAliveNeightboors(int *x, int *y)
{
    int aliveCount = 0;

    for (int xOff = -1; xOff <= 1; xOff++)
        for (int yOff = -1; yOff <= 1; yOff++)
        {
            if (xOff == 0 && yOff == 0) continue;

            int xI = *x + xOff;
            int yI = *y + yOff;

            if (xI < 0) xI = WIDTH - 1;
            if (yI < 0) yI = HEIGHT - 1;

            if (xI >= WIDTH) xI = 0;
            if (yI >= HEIGHT) yI = 0;

            aliveCount += oldCells[GetCellIndex(xI, yI)];
        }
    
    return aliveCount;
}

void MakeNextGeneration()
{
    int x, y;

    for (int i = 0; i < SIZE; i++)
    {
        GetCellCoordinates(i, &x, &y);

        int aliveCount = GetAliveNeightboors(&x, &y);

        if (aliveCount == 3)
            cells[i] = true;
        else if (aliveCount < 2 || aliveCount > 3)
            cells[i] = false;
        
    }
}

void ClearCells()
{
    for (int i = 0; i < SIZE; i++)
        cells[i] = false;
}

void CopyGrid(bool to[], bool from[])
{
    for (int i = 0; i < SIZE; i++)
        to[i] = from[i];
}
#pragma endregion

// ------------------- //

void Start(SDL_Window *window)
{
    SDL_SetWindowSize(
        window,
        WIDTH * CELLSIZE,
        HEIGHT * CELLSIZE
    );

    SDL_SetWindowPosition(
        window,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED
    );
}

void Update(double *dt)
{
    timer += *dt;

    if(!(isPlaying && timer > frameRate)) return;
    timer = 0;

    CopyGrid(oldCells, cells);
    MakeNextGeneration();
}

void Draw(SDL_Renderer *renderer)
{
    for (int x = 0; x < WIDTH; x++)
        for (int y = 0; y < HEIGHT; y++)
        {
            bool isAlive = GetCell(x, y);

            if (isAlive)
            {
                rect.x = x * CELLSIZE;
                rect.y = y * CELLSIZE;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
}

// ------------------- //

#pragma region Mouse
void OnMouseDown(SDL_Event *event)
{
    if (isPlaying) return;

    isDrawing = true;
    SetAtMousePos();
}

void OnMouseReleased(SDL_Event *event)
{
    isDrawing = false;
}

void OnMouseMotion(SDL_Event *event)
{
    if(isDrawing)
        SetAtMousePos();
}
#pragma endregion

#pragma region Keyboard
void OnKeyPressed(SDL_Keycode *key)
{
    switch (*key)
    {
        case SDLK_LALT: shouldErase = true; break;
        case SDLK_SPACE:
            isPlaying = !isPlaying;
            CopyGrid(checkPoint, cells);
            break;
        
        case SDLK_p:
            isPlaying = false;
            CopyGrid(cells, checkPoint);
            break;

        case SDLK_r:
            isPlaying = false;
            ClearCells();
            break;

        case SDLK_UP: frameRate += frameRate < 2 ? 0.01 : 0; break;
        case SDLK_DOWN: frameRate -= frameRate > 0 ? 0.01 : 0; break;
    }
}

void OnKeyUp(SDL_Keycode *key)
{
    switch (*key)
    {
        case SDLK_LALT: shouldErase = false; break;
        default: break;
    }
}
#pragma endregion