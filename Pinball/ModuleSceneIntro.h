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

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* background = nullptr;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	SDL_Texture* elements = nullptr;
	Animation spring_relaxation;
	Animation spring_compression;

};