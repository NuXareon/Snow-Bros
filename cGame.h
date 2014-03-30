#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cMonstre.h"
#include "cData.h"

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <time.h>
using namespace std;

#define SPECIAL_KEY_OFFSET 100 //Offset que sumem a les tecles especial (fletxes, F1, etc...) per evitar solapament amb caracters ASCII

#define GAME_WIDTH	640
#define GAME_HEIGHT 480
#define MAX_LEVEL	9

class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	bool Init();
	bool Loop(bool second_player);
	void Finalize();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadKeyboardSpecial(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	//Process
	bool Process(bool second_player);
	//Output
	void Render(bool second_player);
	void render_string(void* font, const char* string);
	void render_info(bool second_player);

	void UpdatePunts(int x);
	cPlayer GetPlayer();

	void DrawImg(int tex_id, int x, int y, int h, int w);
	void fps();

private:
	unsigned char keys[256];
	cScene Scene;
	cPlayer Player;
	cPlayer Player2;
	cData Data;
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	bool mortPlayer;
	int punts;
	int punts2;
	int maxPunts;
	int maxPunts2;
	int lvl;
	int old_time;
};


