#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball = nullptr;
	int initial_x = 0;
	int initial_y = 0;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	
	//Load ball texture
	ball_texture = App->textures->Load("Assets/Sprites/ball.png");

	//We load the ball
	int initial_x = SCREEN_WIDTH * 0.577;
	int initial_y = SCREEN_HEIGHT * 0.5;
	ball = App->physics->CreateCircle(initial_x, initial_y, 7);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	ball = nullptr;
	ball_texture = nullptr;
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if(ball != nullptr)
	{
		int pos_x;
		int pos_y;

		ball->GetPosition(pos_x, pos_y);
		App->renderer->Blit(ball_texture, pos_x, pos_y);
	}

	return UPDATE_CONTINUE;
}