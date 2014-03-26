#include "cMonstre.h"
#include "cScene.h"

cMonstre::cMonstre() {
	time = 0;	
	atac = false;
	hp = MONSTER_HP;
	regen_cd = MONSTER_REGEN_CD;
	roll_collision = false;
}
cMonstre::~cMonstre(){}

void cMonstre::Draw(int tex_id, int extra_tex_id)
{	
	float xo,yo,xf,yf;
	int tipus;
	GetType(&tipus);
	float t;
	if(tipus==1) t=0.0f;
	else t=0.5;
	int state = GetState();

	switch(state)
	{
		//1
		case STATE_LOOKLEFT:	xo = 0.0f;	yo = 0.125f+t;
								break;
		//4
		case STATE_LOOKRIGHT:	xo = 0.0f;yo = 0.25f+t;
								break;
		//1..3
		case STATE_WALKLEFT:	xo = 0.0f + (GetFrame()*0.125f);yo = 0.125f+t;
								NextFrame(2);
								break;
		//4..6
		case STATE_WALKRIGHT:	xo = 0.0f + (GetFrame()*0.125f); yo = 0.25f+t;
								NextFrame(2);
								break;
		//
		case STATE_JUMPINGL:	xo = 0.25f; yo = 0.25f+t;
								break;
		case STATE_JUMPINGR:	xo = 0.375f; yo = 0.25f+t;
								break;
		case STATE_CAUREL:		xo = 0.25f; yo = 0.375f+t;
								break;
		case STATE_CAURER:		xo = 0.375f; yo = 0.375f+t;
								break;
		// tipus 2
		case STATE_ATACL:		xo = 0.25f; yo = 0.625f;
								if(GetFrame() == 5) atac = false;;
								NextFrame(6);
								break;
		case STATE_ATACR:		xo = 0.375f; yo = 0.625f;
								if(GetFrame() == 5) atac = false;
								NextFrame(6);
								break;

		default:				// Frozen!
								if (state >= STATE_FREEZE_L1 && state <= STATE_FREEZE_L4 || state == STATE_ROLLINGL)
								{
									xo = 0.0f+(GetFrame()*0.125f); yo = 0.375f+t;
									NextFrame(2);
								}
								else if (state >= STATE_FREEZE_R1 && state <= STATE_FREEZE_R4 || state == STATE_ROLLINGR)
								{
									xo = 0.0f+(GetFrame()*0.125f); yo = 0.5f+t;
									NextFrame(2);
								}
								break;
	}
	xf = xo + 0.125f;
	yf = yo - 0.125f;

	DrawRect(tex_id,xo,yo,xf,yf);

	if (state >= STATE_FREEZE_L1 && state <= STATE_FREEZE_R4 || state == STATE_ROLLINGL || state == STATE_ROLLINGR)
	{
		if (state == STATE_FREEZE_L1 || state == STATE_FREEZE_R1) xo = 0.0f; yo = 0.75f;
		if (state == STATE_FREEZE_L2 || state == STATE_FREEZE_R2) xo = 0.25f; yo = 0.75f;
		if (state == STATE_FREEZE_L3 || state == STATE_FREEZE_R3) xo = 0.50f; yo = 0.75f;
		if (state == STATE_FREEZE_L4 || state == STATE_FREEZE_R4 || state == STATE_ROLLINGL || state == STATE_ROLLINGR) xo = 0.75f; yo = 0.75f;

		xf = xo + 0.25f;
		yf = yo - 0.25f;

		DrawRect(extra_tex_id,xo,yo,xf,yf);
	}
}

void cMonstre::AI(int *map)
{
	int x = rand()%100;
	int tipus,tfi;
	GetType(&tipus);
	if ((GetState() < STATE_FREEZE_L1 || GetState() > STATE_FREEZE_R4) && GetState() != STATE_ROLLINGL && GetState() != STATE_ROLLINGR) {
		if(x<2 && tipus==2) {
			if(GetState() == STATE_WALKLEFT){
				atac = true;
				SetState(STATE_ATACL);
			}
			else if (GetState() == STATE_WALKRIGHT){
				atac = true;
				SetState(STATE_ATACR);
			}
		}
		if(!atac){
			time = (time+1)%100;
			if(time > 50) MoveLeft(map);
			else MoveRight(map);
		}
	}
	else if (GetState() == STATE_ROLLINGL) 
	{
		RollLeft(map);
	}
	else if (GetState() == STATE_ROLLINGR) RollRight(map);
}
void cMonstre::Freeze()
{
	int state = GetState();
	if ((state <= STATE_CAUREL) && hp < MONSTER_HP)
	{
		SetState(STATE_FREEZE_L1);
	}
	else if ((state <= STATE_CAURER) && hp < MONSTER_HP)
	{
		SetState(STATE_FREEZE_R1);
	}
	else {
		switch (state)
		{
			case STATE_FREEZE_R1:	if (hp < 2*MONSTER_HP/3) SetState(STATE_FREEZE_R2);
									else if (hp >= MONSTER_HP) SetState(STATE_LOOKRIGHT);
									break;
			case STATE_FREEZE_R2:	if (hp < MONSTER_HP/3) SetState(STATE_FREEZE_R3);
									else if (hp > 2*MONSTER_HP/3) SetState(STATE_FREEZE_R1);
									break;
			case STATE_FREEZE_R3:	if (hp <= 0) SetState(STATE_FREEZE_R4);
									else if (hp > MONSTER_HP/3) SetState(STATE_FREEZE_R2);
									break;
			case STATE_FREEZE_R4:	if (hp > 0) SetState(STATE_FREEZE_R3);
									break;
			case STATE_FREEZE_L1:	if (hp < 2*MONSTER_HP/3) SetState(STATE_FREEZE_L2);
									else if (hp >= MONSTER_HP) SetState(STATE_LOOKLEFT);
									break;
			case STATE_FREEZE_L2:	if (hp < MONSTER_HP/3) SetState(STATE_FREEZE_L3);
									else if (hp > 2*MONSTER_HP/3) SetState(STATE_FREEZE_L1);
									break;
			case STATE_FREEZE_L3:	if (hp == 0) SetState(STATE_FREEZE_L4);
									else if (hp > MONSTER_HP/3) SetState(STATE_FREEZE_L2);
									break;
			case STATE_FREEZE_L4:	if (hp > 0) SetState(STATE_FREEZE_L3);
									break;
		}
	}
}
void cMonstre::DecreaseHP(int x)
{
	hp -= x;
	if (hp < 0) {
		hp = 0;
		regen_cd = 10*MONSTER_REGEN_CD;
	}
	else regen_cd = MONSTER_REGEN_CD;
}
void cMonstre::Regen()
{
	regen_cd -= 1;
	if (regen_cd == 0)
	{
		hp += MONSTER_REGEN_AMOUNT;
		if (hp > MONSTER_HP) hp = MONSTER_HP;
		regen_cd = MONSTER_REGEN_CD;
		Freeze();
	}
}
void cMonstre::Roll(bool left)
{
	if (left) SetState(STATE_ROLLINGL);
	else SetState(STATE_ROLLINGR);
}
void cMonstre::RollingCollisions(std::vector<cMonstre>* monsters)
{
	int tile_x,tile_y;
	int i;
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

	for (i=0; i<(*monsters).size(); ++i)
	{
		(*monsters)[i].GetPosition(&xm,&ym);
		(*monsters)[i].GetWidthHeight(&wm,&hm);
		int mstate = (*monsters)[i].GetState();
		if ((abs((x+w/2)-(xm+wm/2)-5) < (w+wm)/2) && (abs((y+h/2)-(ym+hm/2)-5) < (h+hm)/2) && !(mstate == STATE_ROLLINGL || mstate == STATE_ROLLINGR))
		{
			(*monsters)[i].SetRollCollision(true);
		}
	}	
}
void cMonstre::SetRollCollision(bool rc)
{
	roll_collision = rc;
}
bool cMonstre::GetRollCollision()
{
	return roll_collision;
}