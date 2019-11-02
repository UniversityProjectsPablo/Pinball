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
	void Shape_Map();
	void restartGame();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> chains;

	PhysBody* sensor;
	bool sensed;

	PhysBody* flipper_left;
	PhysBody* flipper_right;
	PhysBody* flipper_left_joint;
	PhysBody* flipper_right_joint;

	Animation spring_relaxation;
	Animation spring_compression;

	SDL_Texture* background = nullptr;
	SDL_Texture* elements = nullptr;
	SDL_Texture* game_over_scene = nullptr;
	SDL_Texture* flipper_left_tex = nullptr;
	SDL_Texture* flipper_right_tex = nullptr;

	bool game_over = false;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* chain;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
