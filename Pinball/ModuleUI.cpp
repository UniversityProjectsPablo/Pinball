#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleUI.h"
#include "ModuleInput.h"



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
	points = 000000;
	balls = 0;

	return ret;
}

bool ModuleUI::CleanUp()
{
	LOG("Unloading UI graphics");

	App->font->UnLoad(font_id);
	return true;
}

update_status ModuleUI:: Update()
{
	//Render
	Points();
	Balls();
	
	return UPDATE_CONTINUE;
}


void ModuleUI::Points() 
{	
	App->font->BlitText(SCREEN_WIDTH / 8, SCREEN_HEIGHT/40, font_id,"00000000");


	// if (      )
	
}

void ModuleUI::Balls()
{
	App->font->BlitText(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT / 35, font_id, "0");

	// if (      )
}
