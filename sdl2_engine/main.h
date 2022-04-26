#pragma once

#include <Windows.h>
#include <SDL.h>
#include <glad.h>
#include <iostream>
#include <cassert>
#include <utility>
#include <vector>
#include <string>
#include <map>

#include "GameStateMgr.h"
#include "GameStateList.h"

struct Window
{
	SDL_Window* window = nullptr;
	SDL_GLContext glContext = nullptr;
};

extern Window App;

extern float fps;
extern float fps_cap;
extern float deltaTime;