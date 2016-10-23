#include "MenuManager.h"

MenuManager::MenuManager()
: sp_title(nullptr)
, sp_won(nullptr)
, sp_lost(nullptr)
, sp_menubar1(nullptr)
, sp_menubar2(nullptr)
, sp_menubar3(nullptr)
, sp_play1(nullptr)
, sp_play2(nullptr)
, sp_option1(nullptr)
, sp_option2(nullptr)
, sp_cancel1(nullptr)
, sp_cancel2(nullptr)
, sp_cancel3(nullptr)
, sp_menuplay(nullptr)
, sp_menudebug(nullptr)
, sp_menuplayagain(nullptr)
, sp_menuquit(nullptr)
, sp_menugodmode(nullptr)
, sp_menuammo(nullptr)
, sp_menuback(nullptr)
, mi_menuSelected(0)
, mb_godMode(false)
, mb_infiniteAmmo(false)
, mb_quit(false)
, mb_restart(false)
{
}

MenuManager::~MenuManager()
{
	delete sp_title;
	sp_title = 0;
	delete sp_won;
	sp_won = 0;
	delete sp_lost;
	sp_lost = 0;

	delete sp_menubar1;
	sp_menubar1 = 0;
	delete sp_menubar2;
	sp_menubar2 = 0;
	delete sp_menubar3;
	sp_menubar3 = 0;

	delete sp_play1;
	sp_play1 = 0;
	delete sp_play2;
	sp_play2 = 0;
	delete sp_option1;
	sp_option1 = 0;
	delete sp_option2;
	sp_option2 = 0;
	delete sp_cancel1;
	sp_cancel1 = 0;
	delete sp_cancel2;
	sp_cancel2 = 0;
	delete sp_cancel3;
	sp_cancel3 = 0;

	delete sp_menuplay;
	sp_menuplay = 0;
	delete sp_menudebug;
	sp_menudebug = 0;
	delete sp_menuplayagain;
	sp_menuplayagain = 0;
	delete sp_menuquit;
	sp_menuquit = 0;

	delete sp_menugodmode;
	sp_menugodmode = 0;
	delete sp_menuammo;
	sp_menuammo = 0;
	delete sp_menuback;
	sp_menuback = 0;
}

bool
MenuManager::Initialise(BackBuffer* backBuffer)
{
	sp_title = backBuffer->CreateSprite("assets\\Menu\\title.png");
	sp_won = backBuffer->CreateSprite("assets\\Menu\\won.png");
	sp_lost = backBuffer->CreateSprite("assets\\Menu\\lost.png");

	sp_menubar1 = backBuffer->CreateSprite("assets\\Menu\\menubar.png");
	sp_menubar2 = backBuffer->CreateSprite("assets\\Menu\\menubar.png");
	sp_menubar3 = backBuffer->CreateSprite("assets\\Menu\\menubar.png");

	sp_play1 = backBuffer->CreateSprite("assets\\Menu\\playbutton.png");
	sp_play2 = backBuffer->CreateSprite("assets\\Menu\\playbutton.png");
	sp_option1 = backBuffer->CreateSprite("assets\\Menu\\optionbutton.png");
	sp_option2 = backBuffer->CreateSprite("assets\\Menu\\optionbutton.png");
	sp_cancel1 = backBuffer->CreateSprite("assets\\Menu\\cancelbutton.png");
	sp_cancel2 = backBuffer->CreateSprite("assets\\Menu\\cancelbutton.png");
	sp_cancel3 = backBuffer->CreateSprite("assets\\Menu\\cancelbutton.png");

	sp_menuplay = backBuffer->CreateSprite("assets\\Menu\\menuplay.png");
	sp_menudebug = backBuffer->CreateSprite("assets\\Menu\\menudebug.png");

	sp_menuplayagain = backBuffer->CreateSprite("assets\\Menu\\menuplayagain.png");

	sp_menuquit = backBuffer->CreateSprite("assets\\Menu\\menuquit.png");

	sp_menugodmode = backBuffer->CreateSprite("assets\\Menu\\menugodmode.png");
	sp_menuammo = backBuffer->CreateSprite("assets\\Menu\\menuammo.png");
	sp_menuback = backBuffer->CreateSprite("assets\\Menu\\menuback.png");

	sp_title->SetPos(b2Vec2(200, 50));

	sp_won->SetPos(b2Vec2(200, 50));

	sp_lost->SetPos(b2Vec2(200, 50));

	sp_menubar1->SetPos(b2Vec2(300, 200));

	sp_menubar2->SetPos(b2Vec2(300, 300));

	sp_menubar3->SetPos(b2Vec2(300, 400));

	sp_play1->SetPos(b2Vec2(250, 200));

	sp_play2->SetPos(b2Vec2(250, 300));

	sp_option1->SetPos(b2Vec2(250, 300));

	sp_cancel1->SetPos(b2Vec2(250, 200));

	sp_cancel2->SetPos(b2Vec2(250, 300));

	sp_cancel3->SetPos(b2Vec2(250, 400));

	sp_menuplay->SetPos(b2Vec2(300, 200));

	sp_menudebug->SetPos(b2Vec2(300, 300));

	sp_menuplayagain->SetPos(b2Vec2(300, 200));

	sp_menugodmode->SetPos(b2Vec2(300, 200));

	sp_menuammo->SetPos(b2Vec2(300, 300));

	sp_menuback->SetPos(b2Vec2(300, 400));

	return (true);
}

void 
MenuManager::DrawMenu(GameState* state, BackBuffer* backBuffer)
{
	backBuffer->DrawSprite(*sp_menubar1);
	backBuffer->DrawSprite(*sp_menubar2);
	if (*state == START)
	{
		backBuffer->DrawSprite(*sp_title);

		backBuffer->DrawSprite(*sp_menubar3);

		backBuffer->DrawSprite(*sp_menuplay);
		backBuffer->DrawSprite(*sp_menudebug);

		sp_menuquit->SetPos(b2Vec2(300, 400));

		backBuffer->DrawSprite(*sp_menuquit);

		if (mi_menuSelected == 0)
			backBuffer->DrawSprite(*sp_play1);
		else if (mi_menuSelected == 1)
			backBuffer->DrawSprite(*sp_option1);
		else if (mi_menuSelected == 2)
			backBuffer->DrawSprite(*sp_cancel3);
	}
	else if (*state == DEBUG)
	{
		backBuffer->DrawSprite(*sp_menubar3);

		backBuffer->DrawSprite(*sp_menugodmode);
		backBuffer->DrawSprite(*sp_menuammo);
		backBuffer->DrawSprite(*sp_menuback);

		switch (mi_menuSelected)
		{
		case 0:
			if (mb_godMode)
				backBuffer->DrawSprite(*sp_play1);
			else
				backBuffer->DrawSprite(*sp_cancel1);
			break;
		case 1:
			if (mb_infiniteAmmo)
				backBuffer->DrawSprite(*sp_play2);
			else
				backBuffer->DrawSprite(*sp_cancel2);
			break;
		case 2:
			backBuffer->DrawSprite(*sp_cancel3);
			break;
		}

	}
	else if (*state == PAUSED)
	{
		//TODO Draw Paused Message
		//backBuffer->DrawSprite(*sp_lost);

		//TODO Draw resume game text in first menu box
		backBuffer->DrawSprite(*sp_menuplayagain);

		sp_menuquit->SetPos(b2Vec2(300, 300));

		backBuffer->DrawSprite(*sp_menuquit);

		if (mi_menuSelected == 0)
			backBuffer->DrawSprite(*sp_play1);
		else
			backBuffer->DrawSprite(*sp_cancel2);
	}
	else if (*state == WON || *state == LOST)
	{
		//TODO Draw Win/Lose Message
		if (*state == WON)
			backBuffer->DrawSprite(*sp_won);
		else
			backBuffer->DrawSprite(*sp_lost);

		backBuffer->DrawSprite(*sp_menuplayagain);

		sp_menuquit->SetPos(b2Vec2(300, 300));

		backBuffer->DrawSprite(*sp_menuquit);

		if (mi_menuSelected == 0)
			backBuffer->DrawSprite(*sp_play1);
		else
			backBuffer->DrawSprite(*sp_cancel2);
	}
}

void 
MenuManager::MenuUp(GameState* state)
{
	if (mi_menuSelected > 0)
		mi_menuSelected--;
}

void 
MenuManager::MenuDown(GameState* state)
{
	int max = 2;
	if (*state == WON || *state == LOST)
		max = 1;

	if (mi_menuSelected < max)
		mi_menuSelected++;
}

void 
MenuManager::MenuEnter(GameState* state)
{
	if (*state == START)
	{
		switch (mi_menuSelected)
		{
		case 0:
			*state = PLAYING;
			break;
		case 1:
			*state = DEBUG;
			mi_menuSelected = 0;
			break;
		case 2:
			SetQuit(true);
			break;
		}
	}
	else if (*state == DEBUG)
	{
		switch (mi_menuSelected)
		{
		case 0:
			mb_godMode = !mb_godMode;
			break;
		case 1:
			mb_infiniteAmmo = !mb_infiniteAmmo;
			break;
		case 2:
			*state = START;
			mi_menuSelected = 0;
			break;
		}
	}
	else if (*state == PAUSED)
	{
		switch (mi_menuSelected)
		{
		case 0:
			*state = PLAYING;
			break;
		case 1:
			SetQuit(true);
			break;
		}
	}
	else if (*state == WON || *state == LOST)
	{
		switch (mi_menuSelected)
		{
		case 0:
			SetRestart(true);
			break;
		case 1:
			SetQuit(true);
			break;
		}
	}
}

bool
MenuManager::GodMode()
{
	return mb_godMode;
}

bool
MenuManager::InfiniteAmmo()
{
	return mb_infiniteAmmo;
}

bool
MenuManager::GetRestart()
{
	return mb_restart;
}

bool
MenuManager::GetQuit()
{
	return mb_quit;
}

void
MenuManager::SetQuit(bool b)
{
	mb_quit = b;
}

void
MenuManager::SetRestart(bool b)
{
	mb_restart = b;
}