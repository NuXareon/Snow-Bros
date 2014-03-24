#pragma once

#include "cBicho.h"
#include <stdlib.h>
#include <time.h>

#define MONSTRE_START_CX		3
#define MONSTRE_START_CY		2
#define MONSTER_HP				100
#define MONSTER_REGEN_CD		30
#define MONSTER_REGEN_AMOUNT	10

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

	int regen_cd;
	int time;
	bool atac;
	int tini;
	int hp;
};
