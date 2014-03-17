#pragma once

#include "cBicho.h"
#include "cMonstre.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void Draw(int tex_id);
	bool CollidesMonstre(std::vector<cMonstre> monsters,bool right);
};
