#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "animation.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	PhysBody* sensor;
	PhysBody* flipper_left;
	PhysBody* flipper_right;
	bool sensed;

	Animation spring_relaxation;
	Animation spring_compression;

	SDL_Texture* background = nullptr;
	SDL_Texture* elements = nullptr;
	SDL_Texture* game_over_scene = nullptr;

	bool game_over = false;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
