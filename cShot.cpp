#include "cShot.h"
#include "cScene.h"
#include "Globals.h"

cShot::cShot(void)
{
	ttl = SHOT_TTL;
	jump_alfa = 0;
}
cShot::~cShot(void){}

void cShot::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
	jump_y = y;
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
void cShot::GetDirection(int *dir)
{
	*dir = direction;
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
	int reduction = 1;
	if (ttl < 2*SHOT_TTL/5) {
		reduction = 8*SHOT_STEP_LENGTH/10;
	}
	else if (ttl < 3*SHOT_TTL/5) reduction = 3*SHOT_STEP_LENGTH/10;
	else if (ttl < 4*SHOT_TTL/5) reduction = 2*SHOT_STEP_LENGTH/10;
	
	x -= SHOT_STEP_LENGTH/reduction;
	return CollidesMapWall(map,false);
}
bool cShot::MoveRight(int *map)
{
	int reduction = 1;
	if (ttl < SHOT_TTL/3) reduction = SHOT_STEP_LENGTH/2;
	else if (ttl < 2*SHOT_TTL/3) reduction = SHOT_STEP_LENGTH/4;
	else if (ttl < 3*SHOT_TTL/4) reduction = SHOT_STEP_LENGTH/8;

	x += SHOT_STEP_LENGTH/reduction;
	return CollidesMapWall(map,true);
	
}

bool cShot::Fall(){
	float alfa;

	jump_alfa += SHOT_FALL_STEP;
		

	alfa = ((float)jump_alfa) * 0.017453f;
	y = jump_y + (int)( ((float)JUMP_HEIGHT) * sin(alfa) /8);

	int yaux = jump_y + (int)( ((float)JUMP_HEIGHT) * sin(alfa) /4);
	if (yaux < y) y = yaux;
		
	if(jump_alfa > 270) {
		int jump_alfa_extra = jump_alfa%270;
		int jump_alfa_aux = 270;
		float alfa_extra = ((float)jump_alfa_extra) * 0.017453f;
		alfa = ((float)jump_alfa_aux) * 0.017453f;
		int y_extra = (int)( ((float)JUMP_HEIGHT) * sin(alfa_extra) /8);
		y = jump_y + (int)( ((float)JUMP_HEIGHT) * sin(alfa) /4) - y_extra;
	}

	return true;
}

bool cShot::Logic(int *map)
{
	bool collision = false;
	if (direction == LEFT_DIRECTION) collision = MoveLeft(map);
	else if (direction == RIGHT_DIRECTION) collision = MoveRight(map);
	Fall();
	ttl -= 1;
	return (ttl > 0 && !collision);
}