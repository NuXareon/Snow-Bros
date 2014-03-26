#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cMonstre.h"
#include "cData.h"

#include <stdio.h>
#include <stdlib.h>

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
	bool Loop();
	void Finalize();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadKeyboardSpecial(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	//Process
	bool Process();
	//Output
	void Render();
	void render_string(void* font, const char* string);
	void render_info();
	
private:
	unsigned char keys[256];
	cScene Scene;
	cPlayer Player;
	cData Data;
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	bool mortPlayer;
	int punts;
	int maxPunts;
	int level;
};


