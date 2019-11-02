#ifndef _MODULEUI_H
#define _MODULEUI_H

#include "Module.h"
#include "ModuleFont.h"
#include "Globals.h"

#include "SDL_mixer/include/SDL_mixer.h"

#define SECOND 1000

struct SDL_Texture;

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status Update();
	bool CleanUp();

	void Points();
	void Balls();
	void End_Screen_Points();


public:

	SDL_Texture* graphics1 = nullptr;
	SDL_Texture* graphics2 = nullptr;

	uint font_id;
	uint balls;
	uint points;
	uint timenow;

	bool ScoreAdded = false;
	uint aux = 0;


};

#endif // _MODULEUI_H

