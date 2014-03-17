
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
	int i,j,k;
	int width_tiles,height_tiles;
	int xm, ym;
	int x, y;
	int w,h;
	GetWidthHeight(&w,&h);
	GetPosition(&x,&y);

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	width_tiles  = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;

	int tile_xr = tile_x + width_tiles;
	int tile_xl = tile_x;
	
	for(i=0;i<width_tiles;i++){
		for(j=0;j<height_tiles;j++)
		{
			for(k=0;k<monsters.size();k++){
				monsters[k].GetPosition(&xm,&ym);
				if((xm/ TILE_SIZE)+i == tile_xl && (ym/ TILE_SIZE)+j == tile_y) return true;
				if((xm/ TILE_SIZE)+i == tile_xr && (ym/ TILE_SIZE)+j == tile_y) return true;
			}
		}
	}
	
	return false;
}

