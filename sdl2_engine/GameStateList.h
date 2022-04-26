#ifndef GAME_STATE_LIST_H_
#define GAME_STATE_LIST_H_

// ---------------------------------------------------------------------------
// game state list

enum
{
	// list of all game states 
	GS_GAME = 0,
	GS_MENU,

	// special game state. Do not change
	GS_RESTART,
	GS_QUIT,
	GS_NONE
};

// ---------------------------------------------------------------------------

#endif // GAME_STATE_LIST_H_