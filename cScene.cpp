#include "cScene.h"
#include "Globals.h"
#include <algorithm>

cScene::cScene(void)
{
}

cScene::~cScene(void)
{
}

std::vector<cMonstre> cScene::GetMonsters()
{
	return monsters;
}

cMonstre cScene::GetMonsters(int i)
{
	return monsters[i];
}

bool cScene::LoadLevel(int level)
{
	errno_t err;
	bool res;
	FILE* fd;
	char file[16];
	int i,j,px,py;
	char tile;
	float coordx_tile, coordy_tile;

	res=true;

	if(level<10) sprintf_s(file,"%s0%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf_s(file,"%s%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

	err=fopen_s(&fd,file,"r");
	if(fd==NULL) return false;

	id_DL=glGenLists(1);
	glNewList(id_DL,GL_COMPILE);
		glBegin(GL_QUADS);
	
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf_s(fd,"%c",&tile);
					if(tile==' ')
					{
						//Tiles must be != 0 !!!
						map[(j*SCENE_WIDTH)+i]=0;
					}
					else
					{
						//Tiles = 1,2,3,...
						map[(j*SCENE_WIDTH)+i] = tile-48;

						if(map[(j*SCENE_WIDTH)+i]%2) coordx_tile = 0.0f;
						else						 coordx_tile = 0.5f;
						if(map[(j*SCENE_WIDTH)+i]<3) coordy_tile = 0.0f;
						else						 coordy_tile = 0.5f;

						//BLOCK_SIZE = 24, FILE_SIZE = 64
						// 24 / 64 = 0.375
						glTexCoord2f(coordx_tile       ,coordy_tile+0.375f);	glVertex2i(px           ,py           );
						glTexCoord2f(coordx_tile+0.375f,coordy_tile+0.375f);	glVertex2i(px+BLOCK_SIZE,py           );
						glTexCoord2f(coordx_tile+0.375f,coordy_tile       );	glVertex2i(px+BLOCK_SIZE,py+BLOCK_SIZE);
						glTexCoord2f(coordx_tile       ,coordy_tile       );	glVertex2i(px           ,py+BLOCK_SIZE);
					}
					px+=TILE_SIZE;
				}
				fscanf_s(fd,"%c",&tile); //pass enter
			}

		glEnd();
	glEndList();

	fclose(fd);

	return res;
}

bool cScene::LoadMonsters(int level) {
	errno_t err;
	bool res;
	FILE *fd;
	char file[16];
	int px,py;
	int tex;

	res=true;

	if(level<10) sprintf_s(file,"%s0%d%s",(char *)MONSTER_FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf_s(file,"%s%d%s",(char *)MONSTER_FILENAME,level,(char *)FILENAME_EXT);

	err=fopen_s(&fd,file,"r");
	if(fd==NULL) return false;

	monsters.clear();
	
	while(fscanf_s(fd,"%d",&tex) > 0) // read texture (type of monster)
	{ 
		int b1 = fscanf_s(fd,"%d",&px); // read x position
		int c1 = fscanf_s(fd,"%d",&py); // read y position

		cMonstre* b = new cMonstre();
		cMonstre bb = *b;
		bb.SetType(tex);
		bb.SetPosition(SCENE_Xo+px*TILE_SIZE,SCENE_Yo+py*TILE_SIZE);
		bb.SetWidthHeight(32,32);
		bb.SetState(STATE_LOOKRIGHT);
		monsters.push_back(bb);
	}

	return res;
}

void cScene::Draw(int tex_id)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
}
void cScene::DrawMonsters(int tex_id, int extra_tex_id)
{
	for (unsigned int i = 0; i < monsters.size(); ++i)
	{
		monsters[i].Draw(tex_id, extra_tex_id);
	}
}
int* cScene::GetMap()
{
	return map;
}

void cScene::Logic() 
{
	unsigned int i;
	for (i=0; i < monsters.size(); ++i)
	{
		monsters[i].Logic(map);
		monsters[i].Regen();
		if (monsters[i].GetRollCount() >= MAX_ROLL_COUNT) monsters.erase(monsters.begin()+i);
	}
	for (i=0; i < shots.size(); ++i)
	{
		bool res = shots[i].Logic(map);
		if (!res) shots.erase(shots.begin()+i);
	}
}
void cScene::AI()
{
	for (unsigned int i = 0; i < monsters.size(); ++i)
	{
		monsters[i].AI(map);
	}
}
void cScene::AddShot(int x, int y, int w, int h, int dir)
{
	cShot* s = new cShot();
	cShot ss = *s;
	ss.SetPosition(x,y);
	ss.SetWidthHeight(w,h);
	ss.SetDirection(dir);
	shots.push_back(ss);
}
void cScene::DrawShots(int tex_id)
{
	for (unsigned int i = 0; i < shots.size(); ++i)
	{
		int dir;
		float xo,yo,xf,yf;
		shots[i].GetDirection(&dir);
		if (dir == LEFT_DIRECTION) 
		{
			xo=0.5f;
			yo=0.0f;
		}
		else if (dir == RIGHT_DIRECTION) 
		{
			xo=0.0f;
			yo=0.0f;
		}
		xf=xo+0.25f;
		yf=yo+0.25f;
		shots[i].DrawRect(tex_id,xo,yo,xf,yf);
	}
}
void cScene::ShotCollisions(std::vector<int>* coll)
{
	for (unsigned int i = 0; i < shots.size(); ++i)
	{
		int m = shots[i].CollidesMonstre(monsters);
		if (m > -1)
		{
			shots.erase(shots.begin()+i);
			int state = monsters[m].GetState();
			if (state == STATE_FREEZE_L4 || state == STATE_FREEZE_R4)
			{
				(*coll).push_back(m);
			}
			monsters[m].DecreaseHP(SHOT_DAMAGE);
			monsters[m].Freeze();
		}
	}
}
void cScene::Roll(int i, bool left)
{
	monsters[i].Roll(left);
}
bool RemoveRollingCondition(cMonstre m)
{
	return m.GetRollCollision();
}
void cScene::RollingCollisions()
{
	for (unsigned int i = 0; i < monsters.size(); ++i)
	{
		if (monsters[i].GetState() == STATE_ROLLINGR || monsters[i].GetState() == STATE_ROLLINGL) monsters[i].RollingCollisions(&monsters);
	}
	monsters.erase(std::remove_if(monsters.begin(), monsters.end(),RemoveRollingCondition),monsters.end());
}