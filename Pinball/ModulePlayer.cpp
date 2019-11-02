#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleUI.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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
	ball = App->physics->createCircle(initial_x, initial_y, ball_radius, b2_dynamicBody);

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
				LOG("You lost!");
				App->scene_intro->game_over = true; //It will change scene from game to game_over
			}
			else //You are alive, take another opportunity!
			{
				ball = App->physics->createCircle(initial_x, initial_y, ball_radius, b2_dynamicBody);
			}
		}

		//Flippers
		if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)		
			App->scene_intro->flipper_right->body->ApplyTorque(60.0f, true);
		else
			App->scene_intro->flipper_right->body->ApplyTorque(-15.0f, true);

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			App->scene_intro->flipper_left->body->ApplyTorque(-60.0f, true);
		else
			App->scene_intro->flipper_left->body->ApplyTorque(15.0f, true);
	}

	return UPDATE_CONTINUE;
}

int ModulePlayer::Get_health()
{
	return health;
}

int ModulePlayer::Get_score()
{
	return score;
}

void ModulePlayer::changeHealth(int newValue)
{
	health = newValue;
}

void ModulePlayer::updateScore(int addScore)
{
	score += addScore;
}

void ModulePlayer::resetScore()
{
	score = 0;
}