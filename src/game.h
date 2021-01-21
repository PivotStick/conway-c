#include <SDL2/SDL.h>
#ifndef __GAME_H
#define __GAME_H

void Start(SDL_Window *window);
void Update(double *deltaTime);
void Draw(SDL_Renderer*);

void OnMouseDown(SDL_Event *event);
void OnMouseReleased(SDL_Event *event);
void OnMouseMotion(SDL_Event *event);

void OnKeyPressed(SDL_Keycode *key);
void OnKeyUp(SDL_Keycode *key);

#endif