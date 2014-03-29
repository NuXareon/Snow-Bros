#pragma once

#include "cBicho.h"
#include "cMonstre.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2
#define PLAYER_DEATH_CD		30

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void Draw(int tex_id);
	int CollidesMonstre(std::vector<cMonstre> monsters);
	int CollidesItem(std::vector<std::pair<int,std::pair<int,int> > > items);
	bool CollidesMonstre(cMonstre monster, bool state_freeze = true);
	void Death();

	void SetMort(bool x);
	void GetMort(bool *x);
	void SetVida(int x);
	void GetVida(int *x);
	void GetDeath(bool *x);

private:
	bool mort; 
	bool death;
	int vida;
	int death_cd;
};
