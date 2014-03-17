#include "cGame.h"
#include "Globals.h"


cGame::cGame(void)
{
}

cGame::~cGame(void)
{
}

bool cGame::Init()
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
	res = Data.LoadImage(IMG_BLOCKS,"blocks.png",GL_RGBA);
	if(!res) return false;
	res = Scene.LoadLevel(1);
	if(!res) return false;
	res = Data.LoadImage(IMG_MONSTER,"monstres1.png",GL_RGBA);
	if(!res) return false;
	res = Scene.LoadMonsters(1);
	if(!res) return false;
	//Monstre.SetTile(25,6);
	//Monstre.SetWidthHeight(32,32);
	//Monstre.SetState(STATE_LOOKRIGHT);

	//Player initialization
	res = Data.LoadImage(IMG_PLAYER,"snowbors256.png",GL_RGBA);
	if(!res) return false;
	//Player.SetWidthHeight(64,64);
	Player.SetTile(4,1);
	Player.SetWidthHeight(32,32);
	Player.SetState(STATE_LOOKRIGHT);

	return res;
}

bool cGame::Loop()
{
	bool res=true;

	res = Process();
	if(res) Render();

	return res;
}

void cGame::Finalize()
{
}

//Input
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
}

void cGame::ReadKeyboardSpecial(unsigned char key, int x, int y, bool press)
{
	keys[key+SPECIAL_KEY_OFFSET] = press;
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
}

//Process
bool cGame::Process()
{
	bool res=true;
	int x,y;
	int w,h;

	Player.GetPosition(&x,&y);

	//Process Input
	if(keys[27])	res=false;
	
	if(keys[GLUT_KEY_UP+SPECIAL_KEY_OFFSET])			Player.Jump(Scene.GetMap());
	if(keys['z'])										Scene.AddShot(x,y,16,16,0); //test
	if(keys[GLUT_KEY_LEFT+SPECIAL_KEY_OFFSET])			Player.MoveLeft(Scene.GetMap());
	else if(keys[GLUT_KEY_RIGHT+SPECIAL_KEY_OFFSET])	Player.MoveRight(Scene.GetMap());
	else Player.Stop(Scene.GetMap());
	
	Scene.AI();
	
	//Game Logic
	Player.Logic(Scene.GetMap());
	Scene.Logic();

	// Player-monster collisions (TODO: ficar dincs de cScene.Logic()?)
	bool collision = Player.CollidesMonstre(Scene.GetMonsters(),false);

	if (collision){
		bool a= true;
	}

	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	Scene.Draw(Data.GetID(IMG_BLOCKS));
	Scene.DrawMonsters(Data.GetID(IMG_MONSTER));
	Scene.DrawShots(Data.GetID(IMG_PLAYER));
	Player.Draw(Data.GetID(IMG_PLAYER));

	glutSwapBuffers();
}
