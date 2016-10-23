#ifndef __MENUMANAGER_H__
#define __MENUMANAGER_H__
#include "Sprite.h"
#include "GameStates.h"
#include "BackBuffer.h"

class MenuManager
{
	//Member Methods

public:
	MenuManager();
	~MenuManager();

	bool Initialise(BackBuffer* backBuffer);

	void DrawMenu(GameState* state, BackBuffer* backBuffer);

	void MenuUp(GameState* state);
	void MenuDown(GameState* state);
	void MenuEnter(GameState* state);

	bool GodMode();
	bool InfiniteAmmo();
	bool GetRestart();
	bool GetQuit();

	void SetRestart(bool b);
	void SetQuit(bool b);

protected:

private:

	//Member Data
public:

protected:
	
	Sprite* sp_title;
	Sprite* sp_won;
	Sprite* sp_lost;

	Sprite* sp_menubar1;
	Sprite* sp_menubar2;
	Sprite* sp_menubar3;

	Sprite* sp_play1;
	Sprite* sp_play2;
	Sprite* sp_option1;
	Sprite* sp_option2;
	Sprite* sp_cancel1;
	Sprite* sp_cancel2;
	Sprite* sp_cancel3;

	Sprite* sp_menuplay;
	Sprite* sp_menudebug;

	Sprite* sp_menuplayagain;

	Sprite* sp_menuquit;

	Sprite* sp_menugodmode;
	Sprite* sp_menuammo;
	Sprite* sp_menuback;

	int mi_menuSelected;
	bool mb_godMode;
	bool mb_infiniteAmmo;
	bool mb_restart;
	bool mb_quit;

private:

};

#endif //__MENUMANAGER_H__