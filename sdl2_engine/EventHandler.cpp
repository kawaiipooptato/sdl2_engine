#include "main.h"
#include "EventHandler.h"

namespace EventHandler
{
	KeyboardHandler Keyboard;
	MouseHandler Mouse;

    void PrintEvent(const SDL_Event* event)
    {
        if (event->type == SDL_WINDOWEVENT) {
            switch (event->window.event) {
            case SDL_WINDOWEVENT_SHOWN:
                SDL_Log("Window %d shown", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                SDL_Log("Window %d hidden", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                SDL_Log("Window %d exposed", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_MOVED:
                SDL_Log("Window %d moved to %d,%d",
                    event->window.windowID, event->window.data1,
                    event->window.data2);
                break;
            case SDL_WINDOWEVENT_RESIZED:
                SDL_Log("Window %d resized to %dx%d",
                    event->window.windowID, event->window.data1,
                    event->window.data2);
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                SDL_Log("Window %d size changed to %dx%d",
                    event->window.windowID, event->window.data1,
                    event->window.data2);
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                SDL_Log("Window %d minimized", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                SDL_Log("Window %d maximized", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_RESTORED:
                SDL_Log("Window %d restored", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_ENTER:
                SDL_Log("Mouse entered window %d",
                    event->window.windowID);
                break;
            case SDL_WINDOWEVENT_LEAVE:
                SDL_Log("Mouse left window %d", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                SDL_Log("Window %d gained keyboard focus",
                    event->window.windowID);
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                SDL_Log("Window %d lost keyboard focus",
                    event->window.windowID);
                break;
            case SDL_WINDOWEVENT_CLOSE:
                SDL_Log("Window %d closed", event->window.windowID);
                break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
            case SDL_WINDOWEVENT_TAKE_FOCUS:
                SDL_Log("Window %d is offered a focus", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_HIT_TEST:
                SDL_Log("Window %d has a special hit test", event->window.windowID);
                break;
#endif
            default:
                SDL_Log("Window %d got unknown event %d",
                    event->window.windowID, event->window.event);
                break;
            }
        }
    }

	void EventPolling()
	{
		SDL_Event m_event;
		
		// Get events
		while (SDL_PollEvent(&m_event))
		{
            PrintEvent(&m_event);
			switch (m_event.type)
			{
			case SDL_QUIT:
				SDL_Log("Quit Event Received");
				gGameStateNext = GS_QUIT;
				break;
				
			case SDL_KEYDOWN:
				Keyboard.Update(m_event.key.keysym.scancode, true);
				break;
				
			case SDL_KEYUP:
				Keyboard.Update(m_event.key.keysym.scancode, false);
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				Mouse.ButtonUpdate(m_event.button.button, true);
				break;
			case SDL_MOUSEBUTTONUP:
				Mouse.ButtonUpdate(m_event.button.button, false);
				break;

			case SDL_MOUSEMOTION:
				Mouse.MotionUpdate(m_event.motion.x, m_event.motion.y);
				break;

			case SDL_MOUSEWHEEL:
				Mouse.WheelUpdate(m_event.wheel.x, m_event.wheel.y);
				break;

            case SDL_WINDOWEVENT:
                switch (m_event.window.event)
                {
                case SDL_WINDOWEVENT_EXPOSED:
                    GameStateDraw();
                    // Swap buffers
                    SDL_GL_SwapWindow(App.window);
                    break;
                }
                break;
		
			default:
				break;
			}
		}
	}
}