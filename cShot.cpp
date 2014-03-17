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
bool cShot::Logic()
{
	ttl -= 1;
	return (ttl > 0);
}