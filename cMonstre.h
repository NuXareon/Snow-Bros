#pragma once

#include "cBicho.h"

#define MONSTRE_START_CX		3
#define MONSTRE_START_CY		2

class cMonstre: public cBicho
{
public:
	cMonstre();
	~cMonstre();

	void Draw(int tex_id);
	void AI(int *map);

	int time;
};
