#include "cShot.h"
#include "cScene.h"
#include "Globals.h"

cShot::cShot(void)
{
	ttl = SHOT_TTL;
}
cShot::~cShot(void){}

void cShot::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cShot::GetPosition(int *posx,int *posy)
{
	*posx = x;
	*posy = y;
}
void cShot::SetWidthHeight(int width,int height)
{
	w = width;
	h = height;
}
void cShot::GetWidthHeight(int *width,int *height)
{
	*width = w;
	*height = h;
}
void cShot::SetDirection(int dir)
{
	direction = dir;
}
void cShot::DrawRect(int tex_id,float xo,float yo,float xf,float yf)
{
	int screen_x,screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(xo,yo);	glVertex2i(screen_x  ,screen_y);
		glTexCoord2f(xf,yo);	glVertex2i(screen_x+w,screen_y);
		glTexCoord2f(xf,yf);	glVertex2i(screen_x+w,screen_y+h);
		glTexCoord2f(xo,yf);	glVertex2i(screen_x  ,screen_y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
bool cShot::CollidesMapWall(int *map,bool right)
{
	int tile_x,tile_y;
	int j;
	int width_tiles,height_tiles;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	width_tiles  = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;

	if(right)	tile_x += width_tiles;
	
	for(j=0;j<height_tiles;j++)
	{
		if(map[ tile_x + ((tile_y+j)*SCENE_WIDTH) ] != 0)	return true;
	}
	
	return false;
}
bool cShot::MoveLeft(int *map)
{
	//bool collide = false;

	int reduction = 1;
	if (ttl < SHOT_TTL/3) reduction = SHOT_STEP_LENGTH/2;
	else if (ttl < 2*SHOT_TTL/3) reduction = SHOT_STEP_LENGTH/4;
	
	//Whats next tile?
	//if( (x % TILE_SIZE) == 0)
	//{
		x -= SHOT_STEP_LENGTH/reduction;

		return CollidesMapWall(map,false);
	//}
	//Advance, no problem
	//else
	//{
	//	x -= SHOT_STEP_LENGTH;
	//}

	//return collide;
}
bool cShot::MoveRight(int *map)
{
	//bool collide = false;
	int reduction = 1;
	if (ttl < SHOT_TTL/3) reduction = SHOT_STEP_LENGTH/2;
	else if (ttl < 2*SHOT_TTL/3) reduction = SHOT_STEP_LENGTH/4;

	//Whats next tile?
	//if( (x % TILE_SIZE) == 0)
	//{
		x += SHOT_STEP_LENGTH/reduction;
		return CollidesMapWall(map,true);
	
	//}
	//Advance, no problem
	//else
	//{
	//	x += SHOT_STEP_LENGTH;
		
	//}
	//return collide;
}

bool cShot::Logic(int *map)
{
	bool collision = false;
	if (direction == LEFT_DIRECTION) collision = MoveLeft(map);
	else if (direction == RIGHT_DIRECTION) collision = MoveRight(map);
	ttl -= 1;
	return (ttl > 0 && !collision);
}