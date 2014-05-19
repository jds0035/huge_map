


#include <iostream>
#include <fstream>
#include "windows.h"

#include "SDL.h"
#include <SDL_mixer.h>

#include <string>
#include <time.h>
#include <cstdlib>
#include <cstring>




void renderTexture_rotate(SDL_Texture *texx, SDL_Renderer *renn, int x, int y, int flip, double angle,double scale){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x; //x,y are in index of which board position it's in.. -95 because we need the top left corner, and they're 90 pixel squares... this could be done better
	dst.y = y ;  ///////////////////////////////////////things to change when board changes
	//Query the texture to get its width and height to use
	SDL_QueryTexture(texx, NULL, NULL, &dst.w, &dst.h);
	dst.w=scale*dst.w;
	dst.h=scale*dst.h;
	//SDL_RenderCopy(renn, texx, NULL, &dst);
	dst.x -=dst.w/2;
	dst.y -=dst.h/2;
	//double angle = GetTickCount();
	//angle = angle/(8+std::abs(x-8)+std::abs(y-8));
	//int angle_i = angle;
	//angle_i = angle_i % 360;

	int angle_i=0;
	if(angle!=0)
{
 angle_i=-angle*180/3.14159+90;
}
	
	if(flip==1)
	{
	SDL_RenderCopyEx(renn,texx,NULL,&dst,angle_i,NULL,SDL_FLIP_HORIZONTAL );
	}
	else 
	{
	SDL_RenderCopyEx(renn,texx,NULL,&dst,angle_i,NULL,SDL_FLIP_NONE );
	}

}

void GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;
   // Get a handle to the desktop window
   const HWND hDesktop = GetDesktopWindow();
   // Get the size of screen to the variable desktop
   GetWindowRect(hDesktop, &desktop);
   // The top left corner will have coordinates (0,0)
   // and the bottom right corner will have coordinates
   // (horizontal, vertical)
   horizontal = desktop.right;
   vertical = desktop.bottom;
}


SDL_Event sdl_event;

const int map_width = 2000;
const int map_height = 2000;
const int box_width = 50;
const int box_height = 50;
 int window_width = 500;
 int window_height = 500;
const int x_boxes = map_width/box_width;
const int y_boxes = map_height/box_height;
int move_timestamp=0;
const int x_sprite=60;
const int y_sprite=90;
int kill_count=0;
int peter_traverso=0;

class Dot
{
    public:
        //The dimensions of the dot
        static const int DOT_WIDTH = 40;
        static const int DOT_HEIGHT = 40;

        //Maximum axis velocity of the dot
        static const int DOT_VEL = 10;
        int mVelX, mVelY;
		int type;
		int dead;
		int indexx;


        //Initializes the variables
		Dot()
		{
		mPosX=rand()%(map_width-5*box_width)+2*box_width;
		mPosY=rand()%(map_height-5*box_height)+2*box_height;
		mVelX=3+(rand()%1000)/500.;
		mVelY=3+(rand()%1000)/500.;
		type=2;
		dead=0;
		}
        Dot(int cannon)
		{
			if(cannon==1)
			{
		mPosX=100;
		mPosY=100;
		mVelX=1;
		mVelY=1;
		type=cannon;
		dead=0;
			}
			else if (cannon==0)
			{
		mPosX=800;
		mPosY=400;
		mVelX=0;
		mVelY=0;
		type=cannon;
		dead=0;
			}
			else if (cannon==3)
			{
				
		mPosX=800;
		mPosY=150;
		mVelX=2;
		mVelY=2;
		type=cannon;
		dead=0;

			}

			else if (cannon==2)
			{
		mPosX=800;
		mPosY=400;
		mVelX=0;
		mVelY=0;
		type=cannon;
		dead=0;
			}
		}

        //Takes key presses and adjusts the dot's velocity
        void handleEvent( SDL_Event& e );
		
		double follow(Dot dottyboy)
		{double delta_x=mPosX-dottyboy.getPosX();
		double delta_y=mPosY-dottyboy.getPosY();
		double delta=sqrt(delta_x*delta_x+delta_y*delta_y);
		mVelX=-5*delta_x/delta;
		mVelY=-5*delta_y/delta;
		return delta_x/delta_y;
		}

        //Moves the dot
        void move()
			{
				if (dead!=1)
				{
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if( ( mPosX < box_width ) || ( mPosX  > map_width-box_width ) )
    {
        //Move back
        mPosX -= mVelX;
		if (type==1)
		{
		mVelX*=-0.4;
		}
		if (type==2||type==3)
		{mVelX*=-1;}
    }

    //Move the dot up or down
    mPosY += mVelY;

    //If the dot went too far up or down
    if( ( mPosY < box_height ) || ( mPosY  > map_height-box_height ) )
    {
        //Move back
        mPosY -= mVelY;
		if (type==1)
		{
		mVelY *=-0.4;
		}
		if (type==2||type==3)
		{mVelY *=-1;}
    }
}
		}

		void kill()
		{mPosX=-1000;
		mPosY=-1000;
		mVelY=0;
		mVelX=0;
		dead=1;
		}

		void index(int i)
		{indexx=i;}
		
		int index()
		{return indexx;}

        //Shows the dot on the screen relative to the camera
        void render(SDL_Renderer *ren,SDL_Texture *tex[], int camX, int camY )
		{
			if (dead!=1)
			{
			
			int time = GetTickCount()%1000;
			int canman;
			if(time<500)
			{canman=0;}
			else
			{canman=1;}
			int flippy;
			if(mVelX<0)
			{flippy=1;}
			else
			{flippy=0;}
    renderTexture_rotate(tex[canman],ren,mPosX - camX, mPosY - camY , flippy,0,1);
			}
		}
		
        void render(SDL_Renderer *ren,SDL_Texture *tex, int camX, int camY, Dot cannon , Dot teddy)
		{
			if (dead !=1)
			{
			
			double delta_x=mPosX-cannon.getPosX();
		double delta_y=mPosY-cannon.getPosY();


			int flippy=0;
			if(mVelX<0&&type!=3)
			{flippy=1;}
			else
			{flippy=0;}

			double angle=0;
	if(type==3)
	{
		flippy=1;
		angle=atan(delta_x/delta_y);
	if (angle<0&&delta_y>=0)
	{flippy=0;
	angle+=3.14159;
	}
	
	if(delta_y<0)
	{angle=atan(delta_x/delta_y)+3.14159;
	if(angle>3.14159)
	{flippy=0;
	angle+=3.14159;}
	}
	}
		
    renderTexture_rotate(tex,ren,mPosX - camX, mPosY - camY , flippy ,angle,1);
		}
		}



		void randomize()
		{mPosX=rand()%(map_width-4*box_width)+2*box_width;
		mPosY=rand()%(map_height-4*box_height)+2*box_width;
		}

        //Position accessors
        int getPosX()
		{return mPosX;}
        int getPosY()
		{return mPosY;}

    private:
        //The X and Y offsets of the dot
        int mPosX, mPosY;

        //The velocity of the dot
};



//boxes is the amount of boxes for sure but top box = boxes-1





	int spaces[x_boxes][y_boxes]; 
	  Dot cannon(1);
	  Dot mariah(0);
	  Dot kkk[100];
	  Dot teddy(3);
	  int teddy_flag=0;
	  int mariah_counter=0;
	  int kkk_counter=0;
	  int m = 1;
	  int space_timestamp=0;



int main(int argc, char* argv[])
{

	
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1){
  //      std::cout << SDL_GetError() << std::endl;
        return 1;
    }

	
		Mix_Music *menu_music = NULL;
		Mix_Music *victory_music = NULL;


	  if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    
                }
	  
	  
		   menu_music = Mix_LoadMUS( "born2run.mp3" );
		   victory_music = Mix_LoadMUS( "victory.mp3" );
		   
	
	srand (time(NULL));
	int horizontal=0;
	int vertical=0;
	GetDesktopResolution(horizontal,vertical);
	window_width=horizontal;
	window_height=vertical;

         SDL_Rect camera = { 0, 0, window_width, window_height};

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);



	   SDL_Window *win = nullptr;
    win = SDL_CreateWindow("Struggle", 100, 100, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP );//!!!!!!!!!!!!!!!!!!!!

	
	   SDL_Renderer *ren = nullptr;
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	const int number_of_cannons=2;
	SDL_Texture *nick_cannon_t[number_of_cannons];
	SDL_Surface *nick_cannon_s[number_of_cannons];
	SDL_Texture *mariah_t[number_of_cannons];
	SDL_Surface *mariah_s[number_of_cannons];
	nick_cannon_s[0]=SDL_LoadBMP("nick cannon[0].bmp");
	nick_cannon_s[1]=SDL_LoadBMP("nick cannon[1].bmp");
	
	mariah_s[0]=SDL_LoadBMP("mariah[0].bmp");
	mariah_s[1]=SDL_LoadBMP("mariah[1].bmp");

	if (peter_traverso==1)

	{
	nick_cannon_s[0]=SDL_LoadBMP("peting[0].bmp");
	nick_cannon_s[1]=SDL_LoadBMP("peting[1].bmp");
	
	mariah_s[0]=SDL_LoadBMP("beibs[0].bmp");
	mariah_s[1]=SDL_LoadBMP("beibs[1].bmp");

	}



	for (int i=0;i<number_of_cannons;i++)
	{
	nick_cannon_t[i]=nullptr;
	SDL_SetColorKey(nick_cannon_s[i], SDL_TRUE, 0x000000);
	nick_cannon_t[i]=SDL_CreateTextureFromSurface(ren,nick_cannon_s[i]);
	SDL_SetTextureBlendMode(nick_cannon_t[i],SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(nick_cannon_s[i]);
	
	mariah_t[i]=nullptr;
	SDL_SetColorKey(mariah_s[i], SDL_TRUE, 0x000000);
	mariah_t[i]=SDL_CreateTextureFromSurface(ren,mariah_s[i]);
	SDL_SetTextureBlendMode(mariah_t[i],SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(mariah_s[i]);
	}

	
	SDL_Texture *kkk_t= nullptr;
	SDL_Surface *kkk_s=nullptr;
	kkk_s=SDL_LoadBMP("kkk.bmp");
	if(peter_traverso==1)
	{kkk_s=SDL_LoadBMP("cossack.bmp");}
	SDL_SetColorKey(kkk_s, SDL_TRUE, 0x000000);
	kkk_t=SDL_CreateTextureFromSurface(ren,kkk_s);
	SDL_SetTextureBlendMode(kkk_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(kkk_s);

	
	SDL_Texture *teddy_t= nullptr;
	SDL_Surface *teddy_s=nullptr;
	teddy_s=SDL_LoadBMP("teddy.bmp");
	if(peter_traverso==1)
	{teddy_s=SDL_LoadBMP("oks.bmp");}
	SDL_SetColorKey(teddy_s, SDL_TRUE, 0x000000);
	teddy_t=SDL_CreateTextureFromSurface(ren,teddy_s);
	SDL_SetTextureBlendMode(teddy_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(teddy_s);
	
	SDL_Texture *oprah_t= nullptr;
	SDL_Surface *oprah_s=nullptr;
	oprah_s=SDL_LoadBMP("oprah.bmp");
	if(peter_traverso==1)
	{oprah_s=SDL_LoadBMP("mauer.bmp");}
	SDL_SetColorKey(oprah_s, SDL_TRUE, 0x000000);
	oprah_t=SDL_CreateTextureFromSurface(ren,oprah_s);
	SDL_SetTextureBlendMode(oprah_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(oprah_s);
	
	SDL_Texture *circle_t= nullptr;
	SDL_Surface *circle_s=nullptr;
	circle_s=SDL_LoadBMP("circle.bmp");
	SDL_SetColorKey(circle_s, SDL_TRUE, 0x000000);
	circle_t=SDL_CreateTextureFromSurface(ren,circle_s);
	SDL_SetTextureBlendMode(circle_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(circle_s);
	
	SDL_Texture *space_t= nullptr;
	SDL_Surface *space_s=nullptr;
	space_s=SDL_LoadBMP("space.bmp");
	SDL_SetColorKey(space_s, SDL_TRUE, 0x000000);
	space_t=SDL_CreateTextureFromSurface(ren,space_s);
	SDL_SetTextureBlendMode(space_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(space_s);
	
	
	SDL_Texture *menu_t= nullptr;
	SDL_Surface *menu_s=nullptr;
	menu_s=SDL_LoadBMP("menu.bmp");
	SDL_SetColorKey(menu_s, SDL_TRUE, 0x000000);
	menu_t=SDL_CreateTextureFromSurface(ren,menu_s);
	SDL_SetTextureBlendMode(menu_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(menu_s);
	
	SDL_Texture *victory_t= nullptr;
	SDL_Surface *victory_s=nullptr;
	victory_s=SDL_LoadBMP("victory.bmp");
	SDL_SetColorKey(victory_s, SDL_TRUE, 0x000000);
	victory_t=SDL_CreateTextureFromSurface(ren,victory_s);
	SDL_SetTextureBlendMode(victory_t,SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(victory_s);
	
	SDL_Rect rannon;
	SDL_Rect rariah;
	SDL_Rect rkk;
	SDL_Rect reddy;
	SDL_QueryTexture(nick_cannon_t[0], NULL, NULL, &rannon.w, &rannon.h);
	SDL_QueryTexture(mariah_t[0], NULL, NULL, &rariah.w, &rariah.h);
	SDL_QueryTexture(kkk_t, NULL, NULL, &rkk.w, &rkk.h);
	SDL_QueryTexture(teddy_t, NULL, NULL, &reddy.w, &reddy.h);
		
	for (int i = 0;i<x_boxes;i++)
	{
		for (int j = 0; j<y_boxes;j++)
		{
	spaces[i][j] = (i+j)%2;

	if (j==0||i==0||i==x_boxes-1||j==y_boxes-1)
	{spaces[i][j]=2;}
	
	std::cout<<spaces[i][j]<<" ";
		}
		std::cout<<"\n";
		
	}
	int mode =1;
	int god_nick=0;
	int space_shit=0;
	int hit_test=0;
	Dot kkk_man[20];
	int kill_teddy=0;
	int g=0;//random flag for whether anything's been hit
	int index[20];

	
	  Mix_PlayMusic( menu_music, -1 );

	  if(peter_traverso==1)
	  {
				  SDL_SetRenderDrawColor( ren, 0, 0, 0, 255 );
                SDL_RenderClear( ren );
				  SDL_SetRenderDrawColor( ren, 0, 0, 0, 255 );
	SDL_RenderCopy(ren,menu_t,NULL,NULL);	
	
				SDL_RenderPresent( ren );
		  SDL_Delay(5200);

				

	  }

	while(m==1)
	{
		while (mode==1)
		{

		if(mariah_counter==10)
		{teddy_flag=1;}


		if ((mariah_counter%2==0)&&(kkk_counter<mariah_counter/2.))
		{
			kkk_counter++;
		}

	if (true)
		{
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		
			if (currentKeyStates[ SDL_SCANCODE_UP ] )
                {
			cannon.mVelY -=1;
		move_timestamp=GetTickCount();
		}

		if (currentKeyStates[ SDL_SCANCODE_DOWN ])
		{cannon.mVelY +=1;
		move_timestamp=GetTickCount();
		}
		if (currentKeyStates[ SDL_SCANCODE_LEFT ])
		{cannon.mVelX -=1;
		move_timestamp=GetTickCount();
		}
		if (currentKeyStates[ SDL_SCANCODE_RIGHT ])
		{cannon.mVelX+=1;
		move_timestamp=GetTickCount();
		}
		if (currentKeyStates[ SDL_SCANCODE_SPACE ])
		{space_shit=1;
		}
		else
		{space_shit=0;}

			
		}

		cannon.move();
		if(teddy_flag==1)
		{
		teddy.follow(cannon);
		teddy.move();
		}
		for (int i=0;i<kkk_counter;i++)
		{kkk[i].move();}


		if (sqrt(pow(cannon.getPosX()-mariah.getPosX(),2.0)+pow(cannon.getPosY()-mariah.getPosY(),2.0))<40)
		{
			mariah.randomize();
			mariah_counter++;

			std::cout<<"your current score is: "<<mariah_counter<<"\n";
			if(mariah_counter==31)//////////////////////////////////////////////////////////////////////////////
			{mode=2;
			break;
			
			}


		}	
		
		
		for (int i=0;i<kkk_counter;i++)
		{
		if (sqrt(pow(cannon.getPosX()-kkk[i].getPosX(),2.0)+pow(cannon.getPosY()-kkk[i].getPosY(),2.0))<40)
		{

			std::cout<<"you lose!!!!!!";
			m=2;
			mode=-1;


		}
		}


				if (sqrt(pow(cannon.getPosX()-teddy.getPosX(),2.0)+pow(cannon.getPosY()-teddy.getPosY(),2.0))<60)
		{

			std::cout<<"you lose!!!!!!";
			m=2;
			mode=-1;


		}
		
                camera.x = ( cannon.getPosX() + Dot::DOT_WIDTH / 2 ) - window_width / 2;
                camera.y = ( cannon.getPosY() + Dot::DOT_HEIGHT / 2 ) - window_height/ 2;

				 if( camera.x < 0 )
                { 
                    camera.x = 0;
                }
                if( camera.y < 0 )
                {
                    camera.y = 0;
                }
                if( camera.x > map_width - camera.w )
                {
                    camera.x = map_width - camera.w;
                }
                if( camera.y > map_height- camera.h )
                {
                    camera.y = map_height - camera.h;
                }

				  SDL_SetRenderDrawColor( ren, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( ren );
				
			 SDL_SetRenderDrawColor(ren, 224, 0, 192, 255);

			

			 for (int i=0;i<x_boxes;i++)
			 {
				 for (int j=0;j<y_boxes;j++)
				 {

					 //need some if statements here

					 if(50*i-camera.x> -box_width&& 50*i-camera.x < window_width&&50*j-camera.y> -box_height&& 50*j-camera.y < window_height)
					 {
			 SDL_Rect fillRect = { 50*i-camera.x, 50*j-camera.y, 50, 50 };
			 if(spaces[i][j]==1)
			 {
				 SDL_SetRenderDrawColor( ren, 0, 255, 255, 255 );     
			 }
			 if(spaces[i][j]==0)
			 {
				 SDL_SetRenderDrawColor( ren, 0, 128, 128, 255 );    
			}
			 if(spaces[i][j]==2)
			 {
				 SDL_SetRenderDrawColor( ren, 162, 131, 128, 255 );    
			}
                SDL_RenderFillRect( ren, &fillRect );
					 }
				 }
			 } 
			 
			 for (int i=0;i<window_width;i+=box_width)
			 {

			 SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
					SDL_RenderDrawLine(ren,(-camera.x)%box_width+i,0,(-camera.x)%50+i,window_height); //x1 y1 x2 y2
			 }
			 for (int i=0;i<window_height;i+=box_height)
			 {

			 SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
					SDL_RenderDrawLine(ren,0,(-camera.y)%box_width+i,window_width,(-camera.y)%50+i); //x1 y1 x2 y2
			 }
			 
			  


			 SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
			 
				cannon.render(ren,nick_cannon_t, camera.x, camera.y );
				mariah.render(ren,mariah_t, camera.x, camera.y );
				teddy.render(ren,teddy_t,camera.x,camera.y,cannon,teddy);
				
				for (int i=0;i<kkk_counter;i++)
					if(kkk[i].getPosX()-camera.x>-100&&kkk[i].getPosX()-camera.x<window_width+100&&kkk[i].getPosY()-camera.y>-100&&kkk[i].getPosY()-camera.y<window_width+100)
					{
				{kkk[i].render(ren,kkk_t,camera.x,camera.y,cannon,teddy);
				}
				}



					double facing = 20;
					if (cannon.mVelX<0)
					{facing=-20;}

					if (GetTickCount()-space_timestamp<2000&&space_shit==1)
					{
			 SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
			
			 for (double i=0;i<800;i +=10)
			 {
						if(GetTickCount()-space_timestamp>i)
						{
		SDL_SetTextureColorMod( circle_t, 255, 0, 0 );
			 renderTexture_rotate(circle_t,ren,cannon.getPosX()+facing-camera.x,cannon.getPosY()-camera.y,0,0,i/5000);

						}
			 }
			 if(GetTickCount()-space_timestamp<1000)
			 {
							 hit_test=0;
							 kill_teddy=0;
							g=0;

			 }

						if(GetTickCount()-space_timestamp>1000)
						{
							if(g==0&&GetTickCount()-space_timestamp<1200)
							{
							for(int i=0;i<kkk_counter;i++)
							{
								if(kkk[i].getPosY()<cannon.getPosY()+20&&kkk[i].getPosY()>cannon.getPosY()-20&&((cannon.getPosX()-kkk[i].getPosX())/facing<0))
								{
								kkk_man[hit_test]=kkk[i];
								index[hit_test]=i;
								hit_test+=1;
								kill_count++;
								g=1;
							}	
							}
							if(kill_count>=kkk_counter)
							{
								if(teddy.getPosY()<cannon.getPosY()+30&&teddy.getPosY()>cannon.getPosY()-30&&((cannon.getPosX()-teddy.getPosX())/facing<0))
									{kill_teddy=1;
								g=1;}
							}



							}
							if (hit_test>0)
							{
								for (int j=0;j<hit_test;j++)
								{
							for (int i=-8;i<9;i++)
									{
							
					SDL_RenderDrawLine(ren,cannon.getPosX()+facing-camera.x, cannon.getPosY()-camera.y+i, kkk_man[j].getPosX()-camera.x, kkk_man[j].getPosY()-camera.y );
					
								if(GetTickCount()-space_timestamp>1500&&kkk[index[j]].dead!=1)
								{
								kkk[index[j]].kill();
								
								}
									}
								}

							}
							else if (kill_teddy==1)
							{
								if(GetTickCount()-space_timestamp<1900)
								{
							for (int i=-8;i<9;i++)
									{
					SDL_RenderDrawLine(ren,cannon.getPosX()+facing-camera.x, cannon.getPosY()-camera.y+i, teddy.getPosX()-camera.x, teddy.getPosY()-camera.y );
					SDL_RenderDrawLine(ren,cannon.getPosX()+facing-camera.x, cannon.getPosY()-camera.y+i, teddy.getPosX()-camera.x, teddy.getPosY()-camera.y +2*i);
							}
								}

							if(GetTickCount()-space_timestamp>1900)
							{teddy.kill();
							mode=4;
							break;}
							}
							else
							{ 
								SDL_Rect smee = { cannon.getPosX()+facing-camera.x, cannon.getPosY()-camera.y-8, 100*facing, 16 };
                SDL_RenderFillRect( ren, &smee );
							}
						}


					}
					else
					{space_timestamp=0;}








				
			 SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

				SDL_RenderPresent( ren );

						while(SDL_PollEvent(&sdl_event) > 0)
		{
		 switch(sdl_event.type)
			{
				 
				
				



				case(SDL_KEYDOWN):
				switch(sdl_event.key.keysym.sym)
				{
				 case(SDLK_UP):
				move_timestamp=GetTickCount();
	
					//dude.y_velocity -=1;
				//	keyflag=0;
				 break;

				  case(SDLK_DOWN):
				move_timestamp=GetTickCount();
					//dude.y_velocity +=1;
					//keyflag=1;
				 break;

				  case(SDLK_LEFT):
				move_timestamp=GetTickCount();
	
					//dude.x_velocity -=1;
					//keyflag=2;
				 break;

				  case(SDLK_RIGHT):
				move_timestamp=GetTickCount();
	
					//dude.x_velocity +=1;
					//keyflag=3;
				 break;

				  case(SDLK_SPACE):
					  if(god_nick==1&&((GetTickCount()-space_timestamp>2000)||space_shit==0))
					  {
					  space_timestamp=GetTickCount();
					  }
					  //deal with killing here ./////////////////////////////////////////////////////////////////////
				}
		 
	/*		case(SDL_MOUSEBUTTONDOWN):

			 if(sdl_event.button.button == SDL_BUTTON_LEFT)
			 {
				 xclick=sdl_event.button.x;
				 yclick=sdl_event.button.y;
				 mouse_time=GetTickCount();
				 dude.x_position=xclick;
				 dude.y_position=yclick;
				 dude.x_velocity=0;
				 dude.y_velocity=0;
			 }
			 break;*/
			}
		}

						}

						while(mode==2)
						{
							
                SDL_RenderClear( ren );
				
			 SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
							renderTexture_rotate(nick_cannon_t[0],ren,800,500,0,0,1);
							renderTexture_rotate(oprah_t,ren,500,500,0,0,1);
							
				SDL_RenderPresent( ren );
								SDL_Delay(500);

			 SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
							renderTexture_rotate(oprah_t,ren,500,500,0,0,1);

								 for (int i=0;i<10;i++)
			 {
				 if(peter_traverso!=1)
				 {
							renderTexture_rotate(nick_cannon_t[0],ren,800,500,0,0,1);
			 SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
					SDL_RenderDrawLine(ren,600,500+20*i,800,500); //x1 y1 x2 y2
				 }
			 }

								 if(peter_traverso==1)
								 {
									 for (int i=-10;i<21;i+=2)
										 {
											 for (int j=-20;j<21;j+=2)
												 {
			 SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
													 SDL_RenderDrawLine(ren,520+j,675+i,800,500);
													 for (double z=500;z<2000;z+=50)
													 {
		SDL_SetTextureColorMod( circle_t, 255, 0, 0 );
			 renderTexture_rotate(circle_t,ren,800,500,1,0,z/5000);
													 }
							renderTexture_rotate(nick_cannon_t[0],ren,800,500,1,0,1);
											 }
									 }
								 }
				SDL_RenderPresent( ren );
								SDL_Delay(4000);

								god_nick=1;
								mode=1;
								
			 SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
                SDL_RenderClear( ren );
							renderTexture_rotate(oprah_t,ren,500,500,0,0,1);
							renderTexture_rotate(nick_cannon_t[1],ren,800,500,0,0,1);	
							renderTexture_rotate(space_t,ren,750,700,0,0,1);	
							
							for (double i=0;i<800;i +=10)
			 {
		SDL_SetTextureColorMod( circle_t, 255, 0, 0 );
			 renderTexture_rotate(circle_t,ren,800+35,500,0,0,i/5000);
			 }

			 SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
				SDL_Rect smee = { 800+35,500-8, 100*20, 16 };
                SDL_RenderFillRect( ren, &smee );


							
				SDL_RenderPresent( ren );
								SDL_Delay(4000);
						}


						while(mode==4)
						{

							
	  Mix_PlayMusic( victory_music, -1 );
							  SDL_SetRenderDrawColor( ren, 0, 0, 0, 255 );
                SDL_RenderClear( ren );
				  SDL_SetRenderDrawColor( ren, 0, 0, 0, 255 );
	SDL_RenderCopy(ren,victory_t,NULL,NULL);	
	
				SDL_RenderPresent( ren );
		  SDL_Delay(150000);

						}



	}


	
	for (int i=0;i<number_of_cannons;i++)
	{
		SDL_DestroyTexture(nick_cannon_t[i]);
		SDL_DestroyTexture(mariah_t[i]);
	}
		SDL_DestroyTexture(kkk_t);
		SDL_DestroyTexture(menu_t);
		SDL_DestroyTexture(victory_t);
		SDL_DestroyTexture(oprah_t);
		SDL_DestroyTexture(circle_t);
		SDL_DestroyTexture(space_t);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
	
	Mix_FreeMusic(menu_music);
	Mix_FreeMusic(victory_music);
    menu_music = NULL;
	SDL_Delay(10000);
	    SDL_Quit();

	return 0;


}

