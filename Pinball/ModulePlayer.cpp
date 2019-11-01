#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball_out = false;
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
	initial_x = SCREEN_WIDTH -27;
	initial_y = SCREEN_HEIGHT -300;
	LOG("SCREEN_WIDTH * 0.577 %f", SCREEN_WIDTH * 0.577);
	ball = App->physics->createCircle(initial_x, initial_y, ball_radius);

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
	if (ball != nullptr)
	{
		int pos_x;
		int pos_y;

		ball->GetPosition(pos_x, pos_y);
		App->renderer->Blit(ball_texture, pos_x, pos_y);
	
		//Ball is out of the map
		if (pos_y >= SCREEN_HEIGHT)
		{
			health--;
			if (health <= 0) //Oups, you lost!
			{
				//TODO: What happens when you die?
				LOG("You lost!");
			}else //You are alive, take another opportunity!
			{
				ball = App->physics->createCircle(initial_x, initial_y, ball_radius);
			}
		}
	}

	if(ball_out)
	{
		
	}	

	return UPDATE_CONTINUE;
}