#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//Load animations 

	//Spring compression
	spring_compression.PushBack({ 30, 5, 27, 86 });
	spring_compression.PushBack({ 57, 5, 27, 86 });
	spring_compression.PushBack({ 84, 5, 27, 86 });
	spring_compression.PushBack({ 111, 5, 27, 86 });
	spring_compression.PushBack({ 138, 5, 27, 86 });
	spring_compression.speed = 0.2f;
	spring_compression.loop = false;

	//Spring relaxation
	spring_relaxation.PushBack({ 3, 5, 27, 86 });
	spring_relaxation.speed = 0.2f;
	
	//Flipper left
	flipper_left.PushBack({50,138,100,22}); //Left flipper	
	flipper_left.PushBack({50,184,92,42}); //Left flipper	
	flipper_left.speed = 0.2f;
	flipper_left.loop = false;

	//Flipper left relaxation
	flipper_left_relaxation.PushBack({ 50,93,90,44 });
	flipper_left_relaxation.speed = 0.2f;

	//Flipper right
	flipper_right.PushBack({ 153,138,97,22 }); //Right flipper
	flipper_right.PushBack({ 159,183,91,44 }); //Right flipper
	flipper_right.speed = 0.2f;
	flipper_right.loop = false;

	//Flipper right relaxation
	flipper_right_relaxation.PushBack({ 159,93,91,44 });
	flipper_right_relaxation.speed = 0.2f;

	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{

}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	// Loading the Background 
	background = App->textures->Load("Assets/Sprites/background.png");

	// Loading the elements (flickers, bouncepads, etc)   

	elements = App->textures->Load("Assets/Sprites/elements.png");
	
	// ----------------------------- ANIMATIONS --------------------------------------------
	
	circles.add(App->physics->CreateCircle(SCREEN_WIDTH * 0.577, SCREEN_HEIGHT * 0.4, 7));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	sensor = nullptr;
	background = nullptr;
	elements = nullptr;

	return true;
}

update_status ModuleSceneIntro::PreUpdate()
{
	App->renderer->Blit(background, 0, 0);

	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{		
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}	

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	// spring animation ---------------

	if (App->input->GetKey(SDL_SCANCODE_DOWN) != KEY_REPEAT)
	{
		App->renderer->Blit(elements, SCREEN_WIDTH * 0.9444, SCREEN_HEIGHT * 0.875, &(spring_relaxation.GetCurrentFrame()), 0.01f);
		spring_compression.Reset();
	}
	else
	{
		App->renderer->Blit(elements, SCREEN_WIDTH * 0.9444, SCREEN_HEIGHT * 0.875, &(spring_compression.GetCurrentFrame()), 0.01f);
	}

	//Left flipper
	if(App->input->GetKey(SDL_SCANCODE_LEFT) != KEY_REPEAT)
	{
		App->renderer->Blit(elements, SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.9, &(flipper_left_relaxation.GetCurrentFrame()), 0.01f);
		flipper_left.Reset();
	}else
	{
		App->renderer->Blit(elements, SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.88, &(flipper_left.GetCurrentFrame()), 0.01f);
	}

	//Right flipper
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) != KEY_REPEAT)
	{
		App->renderer->Blit(elements, SCREEN_WIDTH * 0.500, SCREEN_HEIGHT * 0.898, &(flipper_right_relaxation.GetCurrentFrame()), 0.01f);
		flipper_right.Reset();
	}
	else
	{
		App->renderer->Blit(elements, SCREEN_WIDTH * 0.50, SCREEN_HEIGHT * 0.879, &(flipper_right.GetCurrentFrame()), 0.01f);
	}


	return UPDATE_CONTINUE;
}