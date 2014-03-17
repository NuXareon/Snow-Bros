#pragma once

#include "cTexture.h"
#include "Globals.h"
#include <vector>

#define SHOT_STEP_LENGTH		4
#define SHOT_JUMP_HEIGHT		32
#define SHOT_JUMP_STEP			2
#define	LEFT_DIRECTION			0
#define	RIGHT_DIRECTION			1
#define SHOT_TTL				60

class cShot
{
public:
	cShot(void);
	~cShot(void);

	void SetPosition(int x,int y);
	void GetPosition(int *x,int *y);
	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int *w,int *h);
	void SetDirection(int dir);

	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);

	bool Logic();	// true = stay alive, false = dead

private:

	int x,y;
	int w,h;
	int direction;	

	int ttl;
};