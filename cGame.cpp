#include "cGame.h"
#include "Globals.h"


cGame::cGame(void)
{
	mortPlayer = false;
	punts = 0;
	maxPunts = 0;
	lvl = 2;
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
	res = Data.LoadImage(IMG_LOGO,"LogoSnowBros.png",GL_RGBA);
	if(!res) return false;

	res = Data.LoadImage(IMG_BLOCKS,"blocksSB.png",GL_RGBA);
	if(!res) return false;
	res = Scene.LoadLevel(lvl);
	if(!res) return false;
	res = Data.LoadImage(IMG_MONSTER,"monstres12.png",GL_RGBA);
	if(!res) return false;
	res = Scene.LoadMonsters(1);
	if(!res) return false;

	//Player initialization
	res = Data.LoadImage(IMG_PLAYER,"snowbors256.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_SHOT_P,"dispar.png",GL_RGBA);
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
			Scene.AddShot(x,y,w,h,dir,1);
			Player.SetShotCd(SHOT_CD);
			if(dir == LEFT_DIRECTION) Player.SetState(STATE_ATACL);
			else if(dir == RIGHT_DIRECTION)  Player.SetState(STATE_ATACR);
			punts++;
		}
	}
	if(keys['x'] && cd == 0){// monstre Dispara
		Scene.GetMonsters()[1].GetPosition(&x,&y);
		w=h=30;
		if (Scene.GetMonsters()[1].GetState() <= STATE_CAUREL) dir = 0;
		else dir = 1;
		Scene.AddShot(x,y,w,h,dir,2);
	}

	Scene.AI();
	
	//Game Logic
	Player.Logic(Scene.GetMap());
	Scene.Logic();

	// Player-monster collisions (TODO: ficar dincs de cScene.Logic()?)
	int collision = Player.CollidesMonstre(Scene.GetMonsters(),false);
	
	if (collision>=0){
		bool c; Scene.GetMonsters()[collision].GetCongelat(&c);
		if(c){
			Player.SetMort(true);
			Player.SetState(STATE_DEATH);
		}
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
	
	//Atac del monstre
	int xm,ym,wm,hm,dirm;
	cMonstre monstre;
	unsigned int i;
	for (i=0; i < Scene.GetMonsters().size(); ++i){
		monstre = Scene.GetMonsters()[i];
		monstre.GetPosition(&xm,&ym);
		wm=hm=25;
		if (monstre.GetState() == STATE_ATACL) dirm = LEFT_DIRECTION;
		else dirm = RIGHT_DIRECTION;

		bool b; monstre.GetBolaFoc(&b);
		if(b){
			if(dirm==LEFT_DIRECTION){
				int s = -1;
			}
			Scene.AddShot(xm,ym,wm,hm,dirm,2);
			monstre.SetBolaFoc(false);
		}
	}

	//colisió DisparMonstre Player
	std::vector<cShot> shots = Scene.Getshot();
	for (unsigned int i = 0; i < shots.size(); ++i)
	{
		int p = shots[i].CollidesPlayer(Player);
		if(p > -1){
			shots.erase(shots.begin()+i);
			Scene.Setshot(shots);
			Player.SetMort(true);
			Player.SetState(STATE_DEATH_FOC);
		}
	}

	//Interfície gràfica
	Player.GetVida(&vida);
	if(vida == 0){
		Scene.LoadLevel(lvl);
		Player.SetVida(3);
		if(maxPunts<punts) maxPunts = punts;
		punts = 0;
	}
	
	bool next_level = Scene.GetMonsters().size() == 0;
	if (next_level)
	{
		lvl = (lvl%MAX_LEVEL)+1;

		//Scene initialization
		res = Scene.LoadLevel(lvl);
		if(!res) return false;
		res = Scene.LoadMonsters(lvl);
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
	//Scene.DrawItems(Data.GetID(IMG_PLAYER));
	Scene.DrawMonsters(Data.GetID(IMG_MONSTER), Data.GetID(IMG_SHOT_P));
	Scene.DrawShots(Data.GetID(IMG_SHOT_P));
	//Scene.DrawShots(Data.GetID(IMG_SHOT_M));
	Player.Draw(Data.GetID(IMG_PLAYER));

	//DrawImg(Data.GetID(IMG_LOGO),10,10,32,32);

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
	int vida;
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

void cGame::UpdatePunts(int x){
	punts += x;
}

cPlayer cGame::GetPlayer(){
	return Player;
}

void cGame::DrawImg(int tex_id, int x, int y, int h, int w)
{
	int screen_x,screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(0.0f,0.0f);	glVertex2i(screen_x  ,screen_y);
		glTexCoord2f(1.0f,0.0f);	glVertex2i(screen_x+w,screen_y);
		glTexCoord2f(1.0f,1.0f);	glVertex2i(screen_x+w,screen_y+h);
		glTexCoord2f(0.0f,1.0f);	glVertex2i(screen_x  ,screen_y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}