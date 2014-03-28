#pragma once

#include "cTexture.h"
#include "cBicho.h"
#include "cMonstre.h"
#include "cShot.h"
#include <vector>
#include "cPlayer.h"

#define SCENE_Xo		(2*TILE_SIZE)
#define SCENE_Yo		TILE_SIZE
#define SCENE_WIDTH		36
#define SCENE_HEIGHT	28

#define FILENAME		"level"
#define MONSTER_FILENAME	"monsters"
#define FILENAME_EXT	".txt"

#define TILE_SIZE		16
#define BLOCK_SIZE		24

#define DROP_SPEED_RATE			100
#define	DROP_POWER_SHOT_RATE	100
#define SPEED_BUFF_ID			6
#define	POWER_SHOT_BUFF_ID		5

class cScene
{
public:
	cScene(void);
	virtual ~cScene(void);

	std::vector<cMonstre> GetMonsters();
	cMonstre GetMonsters(int i);
	std::vector<cShot> Getshot();
	void Setshot(std::vector<cShot> s);
	void SetPlayer(cPlayer p);

	bool LoadLevel(int level);
	bool LoadMonsters(int level);
	void AddShot(int x, int y, int w, int h, int direction, int t);
	void Draw(int tex_id);
	void DrawItems(int tex_id);
	void DrawMonsters(int tex_id, int extra_tex_id);
	void DrawShots(int tex_id);
	int *GetMap();

	void Logic();

	void AI();

	void ShotCollisions(std::vector<int>* coll);

	void Roll(int i, bool left);
	void RollingCollisions();

	void DropItem(cMonstre m);
	void DropSpeed(cMonstre m);
	void DropPowerShot(cMonstre m);

private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
	std::vector<cMonstre> monsters;			//monster vector for each level
	std::vector<cShot> shots;				//shot vector
	std::vector<std::pair<int,std::pair<int,int> > > items;		//items on the level: (type,(x,y))
	cPlayer player;
};
