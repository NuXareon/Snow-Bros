#include "cGame.h"
#include "Globals.h"


cGame::cGame(void)
{
	mortPlayer = false;
	punts = 0;
	maxPunts = 0;
	level = 1;
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
	res = Data.LoadImage(IMG_BLOCKS,"blocksSB.png",GL_RGBA);
	if(!res) return false;
	res = Scene.LoadLevel(level);
	if(!res) return false;
	res = Data.LoadImage(IMG_MONSTER,"monstres12.png",GL_RGBA);
	if(!res) return false;
	res = Scene.LoadMonsters(level);
	if(!res) return false;

	//Player initialization
	res = Data.LoadImage(IMG_PLAYER,"snowbors256-2.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_SHOT,"dispar.png",GL_RGBA);
	if(!res) return false;
	//Player.SetWidthHeight(64,64);
	Player.SetTile(2,1);
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
	int s,dir;
	int cd;
	int vida;
	w=h=SHOT_SIZE;

	Player.GetPosition(&x,&y);
	s = Player.GetState();

	if (s <= STATE_CAUREL) dir = LEFT_DIRECTION;
	else dir = RIGHT_DIRECTION;

	Player.GetShotCd(&cd);

	//Process Input
	bool mortP;
	Player.GetMort(&mortP);
	
	if(keys[27])	res=false;
	if(!mortP){ //esta viu??
		if(keys[GLUT_KEY_UP+SPECIAL_KEY_OFFSET])			Player.Jump(Scene.GetMap());
		if(keys[GLUT_KEY_LEFT+SPECIAL_KEY_OFFSET])			Player.MoveLeft(Scene.GetMap());
		else if(keys[GLUT_KEY_RIGHT+SPECIAL_KEY_OFFSET])	Player.MoveRight(Scene.GetMap());
		else												Player.Stop(Scene.GetMap());
		if(keys['z'] && cd == 0) 
		{
			Scene.AddShot(x,y,w,h,dir);
			Player.SetShotCd(SHOT_CD);
			if(dir == LEFT_DIRECTION) Player.SetState(STATE_ATACL);
			else if(dir == RIGHT_DIRECTION)  Player.SetState(STATE_ATACR);
		}
	}
	Scene.AI();
	
	//Game Logic
	Player.Logic(Scene.GetMap());
	Scene.Logic();

	bool collision = Player.CollidesMonstre(Scene.GetMonsters());

	if (collision){
		Player.SetMort(true);
		Player.SetState(STATE_DEATH);		
	}

	bool pd;
	Player.GetDeath(&pd);
	if(pd) Player.Death();

	std::vector<int> shot_collisions;
	Scene.ShotCollisions(&shot_collisions);

	if (shot_collisions.size() > 0)
	{
		for (unsigned int i = 0; i < shot_collisions.size(); ++i)
		{
			bool ps_collision = Player.CollidesMonstre(Scene.GetMonsters(shot_collisions[i]), false);
			int b = 4;
			if (ps_collision) 
			{
				int pstate = Player.GetState();
				bool roll_direction_left = ((pstate <= STATE_CAUREL && pstate >= STATE_LOOKLEFT) || pstate == STATE_ATACL);
				Scene.Roll(shot_collisions[i], roll_direction_left);
			}
		}
	}

	Scene.RollingCollisions();

	Player.GetVida(&vida);
	if(vida == 0) 
	{
		Scene.LoadLevel(1);
		Scene.LoadMonsters(1);
		Player.SetVida(3);
	}

	bool next_level = Scene.GetMonsters().size() == 0;
	if (next_level)
	{
		level = (level%MAX_LEVEL)+1;

		//Scene initialization
		res = Scene.LoadLevel(level);
		if(!res) return false;
		res = Scene.LoadMonsters(level);
		if(!res) return false;

		//Player initialization
		Player.SetTile(2,1);
		Player.SetState(STATE_LOOKRIGHT);
	}


	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	Scene.Draw(Data.GetID(IMG_BLOCKS));
	Scene.DrawItems(Data.GetID(IMG_PLAYER));
	Scene.DrawMonsters(Data.GetID(IMG_MONSTER), Data.GetID(IMG_SHOT));
	Scene.DrawShots(Data.GetID(IMG_SHOT));
	Player.Draw(Data.GetID(IMG_PLAYER));

	render_info();

	glutSwapBuffers();
}


// Render a string
void cGame::render_string(void* font, const char* string)
{
	int i,len = strlen(string);
	for(i=0;i<len;i++)
		glutBitmapCharacter(font, string[i]);
}


// Render information
void cGame::render_info()
{
	int fx,vida;
	char buffvida[10], buffpunts[10],buffmaxPunts[10];
	Player.GetVida(&vida);
	itoa(vida,buffvida,10 );
	itoa(punts,buffpunts,10 );
	itoa(maxPunts,buffmaxPunts,10 );
	char *s[]={	"Vida: ", buffvida,
				"Punts: ", buffpunts,
				"Maxima puntuacio: ", buffmaxPunts
			  };
	
	glDisable(GL_TEXTURE_2D);
		glRasterPos2f(GAME_WIDTH/4,GAME_HEIGHT-10);
		render_string(GLUT_BITMAP_HELVETICA_10,s[0]);
		glRasterPos2f(GAME_WIDTH/4 + 30,GAME_HEIGHT-10);
		render_string(GLUT_BITMAP_HELVETICA_10,s[1]);

		glRasterPos2f(GAME_WIDTH/4 + 100,GAME_HEIGHT-10);
		render_string(GLUT_BITMAP_HELVETICA_10,s[2]);
		glRasterPos2f(GAME_WIDTH/4 + 140,GAME_HEIGHT-10);
		render_string(GLUT_BITMAP_HELVETICA_10,s[3]);

		glRasterPos2f(GAME_WIDTH/4 + 200,GAME_HEIGHT-10);
		render_string(GLUT_BITMAP_HELVETICA_10,s[4]);
		glRasterPos2f(GAME_WIDTH/4 + 290,GAME_HEIGHT-10);
		render_string(GLUT_BITMAP_HELVETICA_10,s[5]);
	glEnable(GL_TEXTURE_2D);
}