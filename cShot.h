#pragma once

#include "cTexture.h"
#include "Globals.h"
#include "cMonstre.h"
#include <vector>

#define SHOT_STEP_LENGTH		8
#define SHOT_JUMP_HEIGHT		32
#define SHOT_JUMP_STEP			2
#define SHOT_SIZE				18
#define	LEFT_DIRECTION			0
#define	RIGHT_DIRECTION			1
#define SHOT_TTL				30
#define SHOT_FALL_STEP			12
#define SHOT_DAMAGE				15

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
	void GetDirection(int *dir);

	bool CollidesMapWall(int *map,bool right);

	bool MoveRight(int *map);
	bool MoveLeft(int *map);
	bool Fall();

	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);
	void Draw(int tex_id);

	bool Logic(int *map);	// true = stay alive, false = dead

	int CollidesMonstre(std::vector<cMonstre> monsters);

private:

	int x,y;
	int w,h;
	int direction;	

	int ttl;

	int jump_y;
	int jump_alfa;
};