
#include "cMonstre.h"

cMonstre::cMonstre() {
	time = 0;
}
cMonstre::~cMonstre(){}

void cMonstre::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	switch(GetState())
	{
		//1
		case STATE_LOOKLEFT:	xo = 0.0f;	yo = 0.25f;
								break;
		//4
		case STATE_LOOKRIGHT:	xo = 0.25f;	yo = 0.25f;
								break;
		//1..3
		case STATE_WALKLEFT:	xo = 0.0f + (GetFrame()*0.25f);	yo = 0.25f;
								NextFrame(2);
								break;
		//4..6
		case STATE_WALKRIGHT:	xo = 0.0f + (GetFrame()*0.25f); yo = 0.5f;
								NextFrame(2);
								break;
		//
		case STATE_JUMPINGL:	xo = 0.5f; yo = 0.25f + (GetFrame()*0.25f);
								NextFrame(3);
								break;
		case STATE_JUMPINGR:	xo = 0.75f; yo = 0.25f + (GetFrame()*0.25f);
								NextFrame(3);
								break;
		case STATE_CAUREL:		xo = 0.5f; yo = 0.75f;
								break;
		case STATE_CAURER:		xo = 0.75f; yo = 0.75f;
								break;
	}
	xf = xo + 0.25f;
	yf = yo - 0.25f;

	DrawRect(tex_id,xo,yo,xf,yf);
}

void cMonstre::AI(int *map)
{
	time = (time+1)%100;
	if(time > 50) MoveLeft(map);
	else MoveRight(map);
}