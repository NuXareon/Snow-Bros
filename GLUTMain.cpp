
#include "Globals.h"
#include "cGame.h"
#include "cMenu.h"

//Delete console
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

cGame Game;
cMenu Menu;

void AppRender()
{
	Menu.Render();
	//Game.Render();
}
void AppKeyboard(unsigned char key, int x, int y)
{
	Game.ReadKeyboard(key,x,y,true);
	Menu.ReadKeyboard(key,x,y,true);
}
void AppKeyboardUp(unsigned char key, int x, int y)
{
	Game.ReadKeyboard(key,x,y,false);
	Menu.ReadKeyboard(key,x,y,false);
}
void AppSpecialKeys(int key, int x, int y)
{
	Game.ReadKeyboardSpecial(key,x,y,true);
	Menu.ReadKeyboardSpecial(key,x,y,true);
}
void AppSpecialKeysUp(int key, int x, int y)
{
	Game.ReadKeyboardSpecial(key,x,y,false);
	Menu.ReadKeyboardSpecial(key,x,y,false);

}
void AppMouse(int button, int state, int x, int y)
{
	Game.ReadMouse(button,state,x,y);
}
void AppIdle()
{
	if(Menu.GetMenu() == 3){
		if(!Game.Loop(false)) exit(0);
	}
	else if (Menu.GetMenu() == 2){
		if(!Game.Loop(true)) exit(0);
	}
	else{
		if(!Menu.Loop()) exit(0);
	}
}

void main(int argc, char** argv)
{
	int res_x,res_y,pos_x,pos_y;

	//GLUT initialization
	glutInit(&argc, argv);

	//RGBA with double buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);

	//Create centered window
	res_x = glutGet(GLUT_SCREEN_WIDTH);
	res_y = glutGet(GLUT_SCREEN_HEIGHT);
	pos_x = (res_x>>1)-(GAME_WIDTH>>1);
	pos_y = (res_y>>1)-(GAME_HEIGHT>>1);
	
	glutInitWindowPosition(pos_x,pos_y);
	glutInitWindowSize(GAME_WIDTH,GAME_HEIGHT);
	glutCreateWindow("Snow Bros");

	/*glutGameModeString("800x600:32");
	glutEnterGameMode();*/

	//Make the default cursor disappear
	//glutSetCursor(GLUT_CURSOR_NONE);

	//Register callback functions
	glutDisplayFunc(AppRender);			
	glutKeyboardFunc(AppKeyboard);		
	glutKeyboardUpFunc(AppKeyboardUp);	
	glutSpecialFunc(AppSpecialKeys);	
	glutSpecialUpFunc(AppSpecialKeysUp);
	glutMouseFunc(AppMouse);
	glutIdleFunc(AppIdle);

	//Game initializations
	Game.Init();
	Menu.Init();

	//Application loop
	glutMainLoop();	
}
