#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	int Get_health();
	

private:
	PhysBody* ball;
	SDL_Texture* ball_texture;
	
	int initial_x;
	int initial_y;
	int ball_radius = 7;
	int health = 5;

	bool game_over = false;
};