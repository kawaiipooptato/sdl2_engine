#include "main.h"
#include "EventHandler.h"

// Global Variables
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const int windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

int VSync = 1; // 0 for immediate updates, 1 for updates synchronized with the vertical retrace, -1 for adaptive vsync

Window App;
Uint64 start, end;
float fps, fps_cap = 60, deltaTime;

int main(int argc, char* args[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(args);

	//// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif
	
	// Initialize the system

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
	else
		SDL_Log("SDL video and audio sub-systems initialized successfully\n");

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		SDL_Log("Linear texture filtering not enabled!");

	// Specify OpenGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create window
	App.window = SDL_CreateWindow("SDL_ENGINE_TEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
	SDL_SetWindowResizable(App.window, SDL_FALSE);

	// Create OpenGL context
	App.glContext = SDL_GL_CreateContext(App.window);

	if (App.glContext == NULL)
	{
		SDL_Log("Couldn't create OpenGL context: %s\n", SDL_GetError());
		return(1);
	}
	else
		SDL_Log("OpenGL context created successfully\n");

	// Initialize GLAD
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		SDL_Log("Failed to initialize GLAD");
		return(1);
	}

	// Use VSync
	if (SDL_GL_SetSwapInterval(VSync) < 0)
	{
		SDL_Log("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}
	
	if (!App.window)
	{
		SDL_Log("Failed to open %d x %d window: %s\n ", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		return(1);
	}
	
	// Initialize renderer
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	// Initialize gamestate manager
	GameStateMgrInit(GS_GAME);

	while (gGameStateCurr != GS_QUIT)
	{
		// If not restarting, load the gamestate
		if (gGameStateCurr != GS_RESTART)
		{
			GameStateMgrUpdate();
			GameStateLoad();
		}
		else
			gGameStateNext = gGameStateCurr = gGameStatePrev;

		// Initialize the gamestate
		GameStateInit();

		while (gGameStateCurr == gGameStateNext)
		{
			start = SDL_GetPerformanceCounter();	// Frame Start

			EventHandler::EventPolling();
			if (gGameStateNext == GS_QUIT) { break; }	//	SDL_QUIT EVENT DETECTED

			if (EventHandler::Keyboard.IsKeyDown(SDL_SCANCODE_ESCAPE)) { gGameStateNext = GS_QUIT; }
			if (EventHandler::Mouse.IsMMouseClicked()) { gGameStateNext = GS_QUIT; }

			GameStateUpdate();
			
			GameStateDraw();

			end = SDL_GetPerformanceCounter();		// Frame End

			deltaTime = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

			// Cap FPS
			Uint32 delay = (Uint32)floor(1000.0f / fps_cap - deltaTime);
			deltaTime += delay;
			SDL_Delay(delay);

			// Calculate FPS
			fps = 1.0f / (deltaTime / 1000.0f);
			//std::cout << fps << std::endl;

			// Swap buffers
			SDL_GL_SwapWindow(App.window);

		}

		GameStateFree();

		if (gGameStateNext != GS_RESTART)
			GameStateUnload();

		gGameStatePrev = gGameStateCurr;
		gGameStateCurr = gGameStateNext;
	}

	SDL_Log("Application Exiting...");

	// free the system
	SDL_GL_DeleteContext(App.glContext);
	SDL_DestroyWindow(App.window);
	atexit(SDL_Quit);
	
	return 0;
}