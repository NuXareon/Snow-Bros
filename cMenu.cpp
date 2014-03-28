#include "cMenu.h"
#include "Globals.h"


cMenu::cMenu(void)
{
	posArrow = 3;
	cd = 0;
	menu = -1;
}

cMenu::~cMenu(void)
{
}


bool cMenu::Init()
{
	bool res=true;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,GAME_WIDTH,0,GAME_HEIGHT,0,1);
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	res = Data.LoadImage(IMG_IMG,"LogoSnowBros.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_LOGO,"SnowBrosLogo256.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_MENU,"Menu.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_ARROW,"Arrow.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_INSTRUC,"Instruccions.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_CREDITS,"Credirs.png",GL_RGBA);
	if(!res) return false;

	return res;
}

bool cMenu::Process()
{	
	bool res=true;

	if(keys[27] && cd==0){
		if(menu == 0 || menu == 1) menu = -1;
		else res=false;
		cd = 10;
	}
	if(keys[GLUT_KEY_UP+SPECIAL_KEY_OFFSET] && cd==0){
		posArrow = (posArrow+1)%4;
		cd = 10;
	}
	if(keys[GLUT_KEY_DOWN+SPECIAL_KEY_OFFSET]&& cd==0){
		posArrow = (posArrow+3)%4;
		cd = 10;
	}
	if(keys[13]){ //intro
		
		if(posArrow == ARROW_PLAYER1)	menu = ARROW_PLAYER1;
		if(posArrow == ARROW_PLAYER2)	menu = ARROW_PLAYER2;
		if(posArrow == ARROW_INSTUC)	menu = ARROW_INSTUC;
		if(posArrow == ARROW_CREDITS)	menu = ARROW_CREDITS;

	}

	return res;
}


bool cMenu::Loop()
{
	bool res=true;

	res = Process();
	if(res) Render();

	if(cd>0) cd -= 1;

	return res;
}

//Input
void cMenu::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
}

void cMenu::ReadKeyboardSpecial(unsigned char key, int x, int y, bool press)
{
	keys[key+SPECIAL_KEY_OFFSET] = press;
}

void cMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	if(menu == -1){
		// Logo Snow Bors
		x = GAME_WIDTH/8;
		y = GAME_HEIGHT*5/8;
		h = 100;
		w = 400;
		DrawImg(Data.GetID(IMG_LOGO),x,y,h,w);

		// Imatge Snow Bors
		x = GAME_WIDTH/2;
		y = 0;
		h = 256;
		w = 256;
		DrawImg(Data.GetID(IMG_IMG),x,y,h,w);

		//Menu
		x = 50;
		y = 0;
		h = 256;
		w = 256;	
		DrawImg(Data.GetID(IMG_MENU),x,y,h,w);
		//DrawRect(Data.GetID(IMG_MENU),0.0f,1.0f,1.0f,0.0f);

		//Arrow
		x = 0;
		y = 75+50*posArrow;	// 225-player1-175-player2,125-Ins,75-Cred
		h = 32;
		w = 32;	
		DrawImg(Data.GetID(IMG_ARROW),x,y,h,w);
	}
	if(menu == 0){	//credits
		x = GAME_WIDTH/2-128;
		y = GAME_HEIGHT/4;
		h = 256;
		w = 256;
		DrawImg(Data.GetID(IMG_CREDITS),x,y,h,w);
	}
	if(menu == 1){	//instruction
		x = 0;//GAME_WIDTH/2-128;
		y = 0;//GAME_HEIGHT/4;
		h = 256;
		w = 256;
		DrawImg(Data.GetID(IMG_INSTRUC),x,y,h,w);
	}
	

	glutSwapBuffers();
}


void cMenu::DrawImg(int tex_id, int x, int y, int h, int w)
{
	int screen_x,screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(0.0f,1.0f);	glVertex2i(screen_x  ,screen_y);
		glTexCoord2f(1.0f,1.0f);	glVertex2i(screen_x+w,screen_y);
		glTexCoord2f(1.0f,0.0f);	glVertex2i(screen_x+w,screen_y+h);
		glTexCoord2f(0.0f,0.0f);	glVertex2i(screen_x  ,screen_y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cMenu::DrawRect(int tex_id,float xo,float yo,float xf,float yf)
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

int cMenu::GetMenu()
{
	return menu;
}

void cMenu::SetMenu(int x)
{
	menu = x;
}