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
	bool game_over = false;
	PhysBody* flipper_left;
	PhysBody* flipper_right;
private:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> chains;

	PhysBody* sensor;
	bool sensed;
	
	PhysBody* flipper_left_joint;
	PhysBody* flipper_right_joint;

	//Light that give points
	PhysBody* green_light1;	
	PhysBody* green_light2;
	PhysBody* green_light3;
	PhysBody* green_light4;
	PhysBody* green_light5;
	PhysBody* green_light6;

	bool green_light1_active;
	bool green_light2_active;
	bool green_light3_active;
	bool green_light4_active;
	bool green_light5_active;
	bool green_light6_active;

	PhysBody* red_light1;
	PhysBody* red_light2;
	PhysBody* red_light3;
	PhysBody* red_light4;
	PhysBody* red_light5;
	PhysBody* red_light6;
	PhysBody* red_light7;
	PhysBody* red_light8;
	PhysBody* red_light9;

	bool red_light1_active;
	bool red_light2_active;
	bool red_light3_active;
	bool red_light4_active;
	bool red_light5_active;
	bool red_light6_active;
	bool red_light7_active;
	bool red_light8_active;
	bool red_light9_active;

	Animation spring_relaxation;
	Animation spring_compression;

	SDL_Texture* background = nullptr;
	SDL_Texture* spring = nullptr;
	SDL_Texture* game_over_scene = nullptr;
	SDL_Texture* flipper_left_tex = nullptr;
	SDL_Texture* flipper_right_tex = nullptr;
	SDL_Texture* active_green_bouncer = nullptr;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* chain;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
