#pragma once

#include "cTexture.h"
#include "Globals.h"

//Image array size
#define NUM_IMG		3

//Image identifiers
#define IMG_BLOCKS	0
#define IMG_PLAYER	1
#define IMG_MONSTER	2
#define IMG_SHOT_P	3
#define IMG_SHOT_M	4

#define IMG_LOGO	5
#define IMG_MENU	6
#define IMG_INSTRUC	7
#define IMG_CREDITS	8
#define IMG_ARROW	9
#define IMG_IMG	10
/*
#define IMG_ENEMY1	2
#define IMG_ENEMY2	3
#define IMG_SHOOT	4
...
*/

class cData
{
public:
	cData(void);
	~cData(void);

	int  GetID(int img);
	void GetSize(int img,int *w,int *h);
	bool LoadImage(int img,char *filename,int type = GL_RGBA);

private:
	cTexture texture[NUM_IMG];
};
