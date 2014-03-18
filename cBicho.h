#pragma once

#include "cTexture.h"
#include "Globals.h"
#include <vector>

#define FRAME_DELAY		8
#define STEP_LENGTH		2
#define JUMP_HEIGHT		96
#define JUMP_STEP		4

// Left
#define STATE_LOOKLEFT		0
#define STATE_WALKLEFT		1
#define STATE_JUMPINGL		2
#define STATE_CAUREL		3
// Right
#define STATE_LOOKRIGHT		4
#define STATE_WALKRIGHT		5
#define STATE_JUMPINGR		6
#define STATE_CAURER		7

#define SHOT_CD			15


class cRect
{
public:
	int left,top,
		right,bottom;
};

class cBicho
{
public:
	cBicho(void);
	cBicho(int x,int y,int w,int h);
	~cBicho(void);

	void SetPosition(int x,int y);
	void GetPosition(int *x,int *y);
	void SetTile(int tx,int ty);
	void GetTile(int *tx,int *ty);
	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int *w,int *h);
	void SetType(int i);
	void SetShotCd(int cd);
	int GetShotCd();

	bool Collides(cRect *rc);
	bool CollidesMapWall(int *map,bool right);
	bool CollidesMapFloor(int *map);
	
	void GetArea(cRect *rc);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void Jump(int *map);
	void Stop(int *map);
	void Logic(int *map);

	int  GetState();
	void SetState(int s);

	void NextFrame(int max);
	int  GetFrame();

private:
	int x,y;
	int w,h;
	int state;
	int type;	

	bool jumping;
	int jump_alfa;
	int jump_y;
	bool caure;

	int seq,delay;

	int shot_cd;
};
