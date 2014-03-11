
#include "cPlayer.h"

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
