
#include "cPlayer.h"
#include "cMonstre.h"
#include "cScene.h"

cPlayer::cPlayer() {}
cPlayer::~cPlayer(){}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	switch(GetState())
	{
		//1
		case STATE_LOOKLEFT:	xo = 0.0f;	yo = 0.125f;
								break;
		//4
		case STATE_LOOKRIGHT:	xo = 0.5f;	yo = 0.125f;
								break;
		//1..3
		case STATE_WALKLEFT:	xo = 0.125f + (GetFrame()*0.125f);	yo = 0.125f;
								NextFrame(3);
								break;
		//4..6
		case STATE_WALKRIGHT:	xo = 0.625f + (GetFrame()*0.125f); yo = 0.125f;
								NextFrame(3);
								break;
		//
		case STATE_JUMPINGL:	xo = 0.0f + (GetFrame()*0.125f); yo = 0.25f;
								NextFrame(5);
								break;
		case STATE_JUMPINGR:	xo = 0.0f + (GetFrame()*0.125f); yo = 0.375f;
								NextFrame(5);
								break;
		case STATE_CAUREL:		xo = 0.0f; yo = 0.25f;
								NextFrame(5);
								break;
		case STATE_CAURER:		xo = 0.0f; yo = 0.375f;
								NextFrame(5);
								break;
	}
	xf = xo + 0.125f;
	yf = yo - 0.125f;

	DrawRect(tex_id,xo,yo,xf,yf);
}

//a millorar
bool cPlayer::CollidesMonstre(std::vector<cMonstre> monsters,bool right)
{
	int tile_x,tile_y;
	int i,j;
	unsigned int k;
	int width_tiles,height_tiles;
	int x, y;
	int w,h;
	int xm, ym;
	int wm,hm;
	GetWidthHeight(&w,&h);
	GetPosition(&x,&y);

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	width_tiles  = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;

	int tile_xr = tile_x + width_tiles;
	int tile_xl = tile_x;

	for (i=0; i<monsters.size(); ++i)
	{
		monsters[i].GetPosition(&xm,&ym);
		monsters[i].GetWidthHeight(&wm,&hm);
		if ((abs((x+w/2)-(xm+wm/2)) < (w+wm)/2) && (abs((y+h/2)-(ym+hm/2)) < (h+hm)/2))
		{
			return true;
		}
	}	
	return false;
}

