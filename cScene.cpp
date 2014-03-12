#include "cScene.h"
#include "Globals.h"

cScene::cScene(void)
{
}

cScene::~cScene(void)
{
}

bool cScene::LoadLevel(int level)
{
	bool res;
	FILE *fd;
	char file[16];
	int i,j,px,py;
	char tile;
	float coordx_tile, coordy_tile;

	res=true;

	if(level<10) sprintf(file,"%s0%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf(file,"%s%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

	fd=fopen(file,"r");
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
					fscanf(fd,"%c",&tile);
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
				fscanf(fd,"%c",&tile); //pass enter
			}

		glEnd();
	glEndList();

	fclose(fd);

	return res;
}

bool cScene::LoadMonsters(int level) {
	bool res;
	FILE *fd;
	char file[16];
	int i,j,px,py;
	int tex;
	float coordx_tile, coordy_tile;

	res=true;

	if(level<10) sprintf(file,"%s0%d%s",(char *)MONSTER_FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf(file,"%s%d%s",(char *)MONSTER_FILENAME,level,(char *)FILENAME_EXT);

	fd=fopen(file,"r");
	if(fd==NULL) return false;

	while(fscanf(fd,"%d",&tex) > 0) // read texture (type of monster)
	{ 
		int b1 = fscanf(fd,"%d",&px); // read x position
		int c1 = fscanf(fd,"%d",&py); // read y position

		cBicho* b = new cBicho();
		cBicho bb = *b;
		bb.SetType(tex);
		bb.SetPosition(SCENE_Xo+px*TILE_SIZE,SCENE_Yo+py*TILE_SIZE);
		bb.SetWidthHeight(32,32);
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
void cScene::DrawMonsters(int tex_id){
	for (int i = 0; i < monsters.size(); ++i)
	{
		monsters[i].DrawRect(tex_id,0.0f,0.25f,0.25f,0.0f);
	}
}
int* cScene::GetMap()
{
	return map;
}
void cScene::Logic() 
{
	for (int i = 0; i < monsters.size(); ++i)
	{
		monsters[i].Logic(map);
	}
}
void cScene::AI()
{
	for (int i = 0; i < monsters.size(); ++i)
	{
		monsters[i].AI(map);
	}
}