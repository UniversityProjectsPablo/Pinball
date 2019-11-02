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
	spring = App->textures->Load("Assets/Sprites/spring.png");
	flipper_left_tex = App->textures->Load("Assets/Sprites/flipper_left.png");
	flipper_right_tex = App->textures->Load("Assets/Sprites/flipper_right.png");
	
	game_over_scene = App->textures->Load("Assets/Sprites/game_over.png");

	//Joints
	b2RevoluteJointDef flipper_joint_def_right;
	b2RevoluteJointDef flipper_joint_def_left;
	b2RevoluteJoint* flipper_joint_right;
	b2RevoluteJoint* flipper_joint_left;

	//Flipper colliders
	flipper_left = App->physics->createRectangle(242, 974,100,10, b2_dynamicBody);
	flipper_right = App->physics->createRectangle(369, 974,100,10, b2_dynamicBody);
	
	flipper_left_joint = App->physics->createCircle(200, 974,5, b2_staticBody);
	flipper_right_joint = App->physics->createCircle(403, 974,5, b2_staticBody);
	
	flipper_joint_def_right.Initialize(flipper_right->body, flipper_right_joint->body, flipper_right_joint->body->GetWorldCenter());
	flipper_joint_def_left.Initialize(flipper_left->body, flipper_left_joint->body, flipper_left_joint->body->GetWorldCenter());

	flipper_joint_def_right.lowerAngle = -0.2f * b2_pi;
	flipper_joint_def_right.upperAngle = 0.2f * b2_pi;
	flipper_joint_def_left.lowerAngle = -0.2f * b2_pi;
	flipper_joint_def_left.upperAngle = 0.2f * b2_pi;

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

	background = nullptr;
	spring = nullptr;
	game_over_scene = nullptr;

	App->textures->Unload(background);
	App->textures->Unload(spring);
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
			App->renderer->Blit(spring, SCREEN_WIDTH * 0.9444, SCREEN_HEIGHT * 0.875, &(spring_relaxation.GetCurrentFrame()), 0.01f);
			spring_compression.Reset();
		}
		else
		{
			App->renderer->Blit(spring, SCREEN_WIDTH * 0.9444, SCREEN_HEIGHT * 0.875, &(spring_compression.GetCurrentFrame()), 0.01f);
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

	int shape_1[54] = {
	416, 964,
	431, 959,
	451, 950,
	475, 930,
	492, 914,
	504, 900,
	510, 886,
	512, 871,
	514, 854,
	512, 839,
	504, 817,
	493, 791,
	485, 772,
	484, 753,
	485, 749,
	491, 748,
	523, 824,
	530, 838,
	533, 865,
	532, 888,
	528, 916,
	520, 933,
	509, 947,
	487, 966,
	471, 976,
	432, 993,
	421, 968
	};

	int shape_2[46] = {
	189, 963,
	179, 996,
	146, 980,
	124, 968,
	107, 955,
	89, 936,
	80, 912,
	76, 880,
	77, 845,
	103, 782,
	118, 748,
	124, 750,
	126, 760,
	120, 781,
	109, 805,
	97, 841,
	96, 861,
	98, 880,
	102, 893,
	107, 904,
	118, 916,
	144, 938,
	166, 953
	};

	int shape_3[44] = {
	490, 722,
	522, 721,
	535, 722,
	545, 728,
	555, 737,
	560, 752,
	564, 751,
	572, 745,
	580, 735,
	586, 726,
	591, 710,
	593, 703,
	593, 683,
	582, 666,
	570, 657,
	555, 650,
	540, 645,
	517, 643,
	515, 650,
	507, 674,
	498, 700,
	492, 719
	};

	int shape_4[56] = {
	431, 707,
	395, 655,
	381, 655,
	318, 682,
	310, 682,
	300, 673,
	288, 652,
	282, 623,
	283, 581,
	297, 551,
	317, 532,
	345, 514,
	371, 504,
	400, 495,
	423, 491,
	465, 490,
	464, 516,
	455, 522,
	443, 532,
	434, 542,
	427, 556,
	426, 573,
	427, 588,
	431, 605,
	440, 620,
	450, 627,
	465, 634,
	437, 713
	};

	int shape_5[112] = {
	552, 519,
	552, 465,
	550, 418,
	544, 356,
	535, 318,
	517, 268,
	498, 236,
	473, 213,
	446, 193,
	423, 183,
	393, 174,
	379, 173,
	332, 172,
	301, 177,
	277, 184,
	249, 197,
	226, 213,
	214, 221,
	200, 236,
	191, 249,
	187, 259,
	184, 275,
	182, 286,
	182, 304,
	185, 338,
	189, 353,
	194, 366,
	219, 409,
	215, 417,
	204, 397,
	188, 366,
	182, 346,
	178, 311,
	177, 286,
	180, 266,
	188, 243,
	202, 226,
	217, 212,
	237, 197,
	272, 180,
	301, 172,
	320, 167,
	357, 166,
	392, 168,
	435, 182,
	457, 194,
	471, 203,
	493, 222,
	509, 242,
	520, 261,
	532, 292,
	539, 311,
	548, 360,
	555, 406,
	557, 453,
	558, 525
	};

	int shape_6[142] = {
	154, 448,
	146, 433,
	135, 403,
	129, 375,
	126, 351,
	128, 303,
	134, 270,
	141, 247,
	160, 217,
	181, 193,
	213, 167,
	237, 153,
	280, 137,
	321, 129,
	378, 129,
	414, 134,
	454, 147,
	484, 162,
	503, 174,
	526, 192,
	541, 209,
	552, 226,
	565, 250,
	575, 271,
	582, 291,
	588, 315,
	592, 335,
	593, 361,
	595, 392,
	597, 422,
	597, 450,
	598, 488,
	598, 514,
	598, 529,
	592, 529,
	591, 471,
	591, 422,
	588, 356,
	585, 322,
	578, 295,
	568, 268,
	550, 234,
	536, 215,
	522, 200,
	499, 180,
	476, 165,
	449, 152,
	413, 141,
	395, 136,
	363, 134,
	328, 134,
	314, 136,
	291, 141,
	265, 149,
	240, 161,
	218, 173,
	197, 188,
	183, 201,
	165, 221,
	148, 251,
	142, 265,
	136, 287,
	133, 304,
	132, 326,
	134, 355,
	138, 381,
	142, 398,
	147, 416,
	154, 432,
	156, 439,
	158, 446
	};

	int shape_7[26] = {
	181, 883,
	157, 794,
	159, 782,
	165, 776,
	179, 774,
	188, 778,
	252, 870,
	257, 879,
	258, 890,
	252, 898,
	243, 902,
	209, 897,
	184, 893
	};

	int shape_8[38] = {
	425, 893,
	452, 796,
	453, 789,
	446, 779,
	436, 776,
	424, 778,
	419, 782,
	412, 792,
	403, 807,
	387, 829,
	375, 846,
	361, 867,
	351, 880,
	349, 891,
	358, 900,
	369, 904,
	381, 902,
	403, 898,
	421, 895
	};

	int shape_9[38] = {
	120, 573,
	95, 557,
	85, 551,
	80, 543,
	81, 530,
	88, 523,
	97, 522,
	117, 526,
	147, 535,
	196, 548,
	225, 556,
	233, 558,
	238, 568,
	238, 578,
	203, 614,
	198, 617,
	190, 617,
	181, 613,
	122, 575
	};

	chains.add(App->physics->createChain(0, 0, map, 234));
	chains.add(App->physics->createChain(0, 0, shape_1, 54));
	chains.add(App->physics->createChain(0, 0, shape_2, 46));
	chains.add(App->physics->createChain(0, 0, shape_3, 44));
	chains.add(App->physics->createChain(0, 0, shape_4, 56));
	chains.add(App->physics->createChain(0, 0, shape_5, 112));
	chains.add(App->physics->createChain(0, 0, shape_6, 142));
	chains.add(App->physics->createChain(0, 0, shape_7, 26));
	chains.add(App->physics->createChain(0, 0, shape_8, 38));
	chains.add(App->physics->createChain(0, 0, shape_9, 38));

}

void ModuleSceneIntro::restartGame()
{
	game_over = false;
	App->player->changeHealth(5);
	App->player->resetScore();
}