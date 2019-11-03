#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleUI.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"

#include "SDL/include/SDL.h"


ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled) {

}

ModuleUI::~ModuleUI()
{}

bool ModuleUI::Start()
{
	LOG("Loading UI textures");
	bool ret = true;

	font_id = App->font->Load("Assets/Sprites/Font.png", "0123456789 ", 1);
	return ret;
}

bool ModuleUI::CleanUp()
{
	LOG("Unloading UI graphics");

	App->font->UnLoad(font_id);

	return true;
}

update_status ModuleUI::Update()
{
	//Render if we are on game
	if (App->scene_intro->game_over == true)
	{
		End_Screen_Points();
	}
	else {
		Points();
		Balls();
	}

	return UPDATE_CONTINUE;
}


void ModuleUI::Points()
{
	char temp[10];
	_itoa_s(App->player->Get_score(), temp, 10);

	App->font->BlitText(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 40, font_id, temp);
}

void ModuleUI::Balls()
{
	char temp[10];
	_itoa_s(App->player->Get_health(), temp, 10);

	App->font->BlitText(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT / 35, font_id, temp);

}

void ModuleUI::End_Screen_Points()
{
	char temp[10];
	_itoa_s(App->player->Get_score(), temp, 10);

	App->font->BlitText(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, font_id, temp);

	if (first_round == true)
		;
	else
	{
		char temp2[10];
		_itoa_s(App->player->Get_PrevScore(), temp2, 10);

		App->font->BlitText(SCREEN_WIDTH /2, SCREEN_HEIGHT *0.6, font_id, temp2);
	}

	char temp3[10];
	_itoa_s(App->player->Get_Highscore(), temp3, 10);

	App->font->BlitText(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.7, font_id, temp3);
}

