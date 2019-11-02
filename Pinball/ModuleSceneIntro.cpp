#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
	circle = box = chain = NULL;
	
	// ----------------------------- ANIMATIONS --------------------------------------------

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
	
	Shape_Map();


	// Loading the Background 
	background = App->textures->Load("Assets/Sprites/background.png");
	
	// Loading the elements (flippers, bouncepads, etc)   
	elements = App->textures->Load("Assets/Sprites/elements.png");
	flipper_left_tex = App->textures->Load("Assets/Sprites/flipper_left.png");
	flipper_right_tex = App->textures->Load("Assets/Sprites/flipper_right.png");
	
	game_over_scene = App->textures->Load("Assets/Sprites/game_over.png");

	//Joints
	b2RevoluteJointDef flipper_joint_def_right;
	b2RevoluteJointDef flipper_joint_def_left;
	b2RevoluteJoint* flipper_joint_right;
	b2RevoluteJoint* flipper_joint_left;

	//Flipper colliders
	flipper_left = App->physics->createRectangle(242, 984,100,25, b2_dynamicBody);
	flipper_right = App->physics->createRectangle(372, 984,100,25, b2_dynamicBody);
	
	flipper_left_joint = App->physics->createCircle(242, 984,5, b2_staticBody);
	flipper_right_joint = App->physics->createCircle(372, 984,5, b2_staticBody);
	
	flipper_joint_def_right.Initialize(flipper_right->body, flipper_right_joint->body, flipper_right_joint->body->GetWorldCenter());
	flipper_joint_def_left.Initialize(flipper_left->body, flipper_left_joint->body, flipper_left_joint->body->GetWorldCenter());

	flipper_joint_def_right.lowerAngle = -0.12f * b2_pi;
	flipper_joint_def_right.upperAngle = 0.25f * b2_pi;
	flipper_joint_def_left.lowerAngle = -0.12f * b2_pi;
	flipper_joint_def_left.upperAngle = 0.25f * b2_pi;

	flipper_joint_def_right.enableLimit = true;
	flipper_joint_def_left.enableLimit = true;

	flipper_joint_right = (b2RevoluteJoint*)App->physics->world->CreateJoint(&flipper_joint_def_right);
	flipper_joint_left = (b2RevoluteJoint*)App->physics->world->CreateJoint(&flipper_joint_def_left);
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	sensor = nullptr;

	App->textures->Unload(background);
	App->textures->Unload(elements);
	App->textures->Unload(game_over_scene);
	App->textures->Unload(flipper_left_tex);
	App->textures->Unload(flipper_right_tex);

	return true;
}

update_status ModuleSceneIntro::PreUpdate()
{
		
	if(game_over == false)	
		App->renderer->Blit(background, 0, 0);
	else if(game_over == true)
	{
		//Change scene
		App->renderer->Blit(game_over_scene, 0, 0);
	}
	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if(game_over == false)
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
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
		if (ray_on == true)
		{
			fVector destination(mouse.x - ray.x, mouse.y - ray.y);
			destination.Normalize();
			destination *= ray_hit;

			App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

			if (normal.x != 0.0f)
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

		//Blit flippers

		if(flipper_left != NULL)
		{
			int x, y;
			flipper_left->GetPosition(x, y);
			App->renderer->Blit(flipper_left_tex, x, y, NULL, 0.2f, flipper_left->GetRotation());
		}
		if (flipper_right != NULL)
		{
			int x, y;
			flipper_right->GetPosition(x, y);
			App->renderer->Blit(flipper_right_tex, x, y, NULL, 0.2f, flipper_right->GetRotation());
		}

	}else if(game_over == true)
	{
		LOG("aqui");
		if(App->input->GetKey(SDL_SCANCODE_SPACE) || App->input->GetKey(SDL_SCANCODE_KP_ENTER))
		{
			LOG("dfasdfdas");
			restartGame();
		}
	}

	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::Shape_Map() {

	p2List_item<PhysBody*>* temp = chains.getFirst();
	while (temp != NULL) {
		b2Body* body = temp->data->body;
		temp->data->body->GetWorld()->DestroyBody(body);
		temp = temp->next;
	}
	chains.clear();

	int map[234] = {
	445, 1079,
	445, 1054,
	550, 988,
	572, 965,
	587, 935,
	592, 907,
	592, 529,
	597, 529,
	597, 489,
	603, 489,
	603, 483,
	612, 483,
	611, 906,
	640, 906,
	639, 483,
	641, 483,
	640, 412,
	638, 385,
	636, 355,
	631, 321,
	624, 288,
	614, 251,
	600, 220,
	578, 185,
	547, 149,
	529, 133,
	495, 110,
	466, 97,
	455, 93,
	414, 84,
	329, 72,
	234, 84,
	205, 92,
	177, 101,
	159, 110,
	139, 121,
	118, 135,
	96, 152,
	85, 162,
	72, 173,
	56, 194,
	47, 208,
	47, 244,
	56, 263,
	74, 296,
	87, 321,
	99, 346,
	96, 348,
	88, 334,
	55, 269,
	43, 244,
	43, 108,
	43, 91,
	40, 85,
	36, 82,
	30, 82,
	26, 82,
	20, 86,
	16, 92,
	16, 114,
	16, 140,
	16, 186,
	16, 259,
	32, 301,
	54, 352,
	62, 368,
	52, 374,
	44, 349,
	18, 277,
	11, 300,
	8, 316,
	5, 339,
	4, 356,
	4, 387,
	4, 414,
	6, 441,
	10, 475,
	18, 518,
	29, 562,
	43, 597,
	51, 611,
	85, 636,
	97, 643,
	104, 651,
	108, 661,
	109, 670,
	109, 681,
	105, 694,
	93, 718,
	85, 729,
	77, 733,
	70, 732,
	56, 727,
	46, 726,
	38, 726,
	32, 728,
	26, 732,
	20, 742,
	17, 759,
	17, 796,
	17, 836,
	17, 899,
	19, 916,
	23, 933,
	27, 946,
	34, 960,
	44, 974,
	54, 984,
	70, 995,
	88, 1006,
	164, 1054,
	164, 1080,
	0, 1080,
	0, 0,
	648, 0,
	648, 1080,
	450, 1080
	};

	chains.add(App->physics->createChain(0, 0, map, 234));

}

void ModuleSceneIntro::restartGame()
{
	game_over = false;
	App->player->changeHealth(5);
	App->player->resetScore();
}