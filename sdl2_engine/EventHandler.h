#ifndef EVENT_HANDLER_H_
#define EVENT_HANDLER_H_

namespace EventHandler
{
	class KeyboardHandler
	{
	public:
		void Update(SDL_Scancode scancode, bool state)
		{
			prev_keyStates[scancode] = curr_keyStates[scancode];
			curr_keyStates[scancode] = state;
		}

		bool IsKeyDown(SDL_Scancode scancode)
		{
			return curr_keyStates[scancode];
		}		
		bool IsKeyUp(SDL_Scancode scancode)
		{
			return !curr_keyStates[scancode];
		}
		bool IsKeyPressed(SDL_Scancode scancode)
		{
			return curr_keyStates[scancode] && prev_keyStates[scancode];
		}
		
	private:
		std::map<SDL_Scancode, bool> curr_keyStates;
		std::map<SDL_Scancode, bool> prev_keyStates;
	};

	class MouseHandler
	{
	public:
		void ButtonUpdate(Uint8 scancode, bool state)
		{
			prev_mouseStates[scancode] = curr_mouseStates[scancode];
			curr_mouseStates[scancode] = state;
		}

		void MotionUpdate(Sint32 x_coord, Sint32 y_coord)
		{
			rel_pos.first = x_coord - pos.first;
			rel_pos.second = y_coord - pos.second;
			
			pos.first = x_coord;
			pos.second = y_coord;
		}

		void WheelUpdate(Sint32 hor_scroll, Sint32 vert_scroll)
		{
			wheel.first = hor_scroll;
			wheel.second = vert_scroll;
		}

		bool IsLMouseClicked()
		{
			return curr_mouseStates[SDL_BUTTON_LEFT];
		}

		bool IsMMouseClicked()
		{
			return curr_mouseStates[SDL_BUTTON_MIDDLE];
		}
		
		bool IsRMouseClicked()
		{
			return curr_mouseStates[SDL_BUTTON_RIGHT];
		}
		
		bool IsLMousePressed()
		{
			return curr_mouseStates[SDL_BUTTON_LEFT] && prev_mouseStates[SDL_BUTTON_LEFT];
		}

		bool IsMMousePressed()
		{
			return curr_mouseStates[SDL_BUTTON_MIDDLE] && prev_mouseStates[SDL_BUTTON_MIDDLE];
		}

		bool IsRMousePressed()
		{
			return curr_mouseStates[SDL_BUTTON_RIGHT] && prev_mouseStates[SDL_BUTTON_RIGHT];
		}

		std::pair<Sint32, Sint32> GetMouseRel()
		{
			return rel_pos;
		}

		std::pair<Sint32, Sint32> GetMousePos()
		{
			return pos;
		}

		// y > 0: scroll up
		// y < 0: scroll down
		// x > 0: scroll right
		// x < 0: scroll left
		std::pair<Sint32, Sint32> GetMouseWheel()
		{
			return wheel;
		}
		
	private:
		std::pair<Sint32, Sint32> pos;
		std::pair<Sint32, Sint32> rel_pos;
		std::pair<Sint32, Sint32> wheel;
		std::map<Uint8, bool> curr_mouseStates;
		std::map<Uint8, bool> prev_mouseStates;
	};
	
	void EventPolling();

	extern KeyboardHandler Keyboard;
	extern MouseHandler Mouse;
}

#endif // EVENT_HANDLER_H_