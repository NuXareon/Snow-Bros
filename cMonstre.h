#pragma once

#include "cBicho.h"

#define MONSTRE_START_CX		3
#define MONSTRE_START_CY		2
#define MONSTER_HP				100
#define MONSTER_REGEN_CD		60
#define MONSTER_REGEN_AMOUNT	5

class cMonstre: public cBicho
{
public:
	cMonstre();
	~cMonstre();

	void Draw(int tex_id, int extra_tex_id);
	void AI(int *map);
	void Freeze();
	void DecreaseHP(int x);
	void Regen();

private:
	int regen_cd;
	int hp;
	int time;
};
