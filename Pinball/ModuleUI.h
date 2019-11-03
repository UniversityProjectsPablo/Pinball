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

	uint font_id;
	bool first_round = true;

};

#endif // _MODULEUI_H

