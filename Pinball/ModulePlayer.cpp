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
	initial_x = SCREEN_WIDTH  -27;
	initial_y = SCREEN_HEIGHT  -200;
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
	//Launch ball
	if(ball_launched == false && App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		ball_launched = true;

		b2Vec2 impulse = b2Vec2(0.0f, -1.0f);
		
		b2Vec2 point = ball->body->GetLocalCenter();
		ball->body->ApplyLinearImpulse(impulse, point, true);
	}

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
			ball_launched = false;
			if (health <= 0) //Oups, you lost!
			{	
				changeHighscore(score);
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
	
		teleport(); //We teleport the ball if it enters the hole
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

int ModulePlayer::Get_PrevScore()
{
	return prev_score;
}

int ModulePlayer::Get_Highscore()
{
	return highscore;
}

void ModulePlayer::changeHealth(int newValue)
{
	health = newValue;
}

void ModulePlayer::changePrevScore(int newValue)
{
	prev_score = newValue;
}

void ModulePlayer::changeHighscore(int newValue)
{
	if (newValue > highscore)
		highscore = newValue;
	else
		;
}

void ModulePlayer::updateScore(int addScore)
{
	score += addScore;
}

void ModulePlayer::resetScore()
{
	score = 0;
}

void ModulePlayer::teleport()
{
	int x, y;
	ball->GetPosition(x,y);

	if(x > 550 && x < 580)
		if(y > 625 && y < 650)
		{ 
			//Ball is inside the hole area. So we teleport it to the other hole
			ball = nullptr; //We delete the current ball
			ball = App->physics->createCircle(25, 100, ball_radius, b2_dynamicBody); //We create a new ball, simulating a teleport
		}
}