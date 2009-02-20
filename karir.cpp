#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <math.h>

#include <SDL.h>
#include <SDL_rotozoom.h>
#include <SDL_draw.h>
#include <SDL_framerate.h>

#include "CSurface.h"
#include "CEvent.h"
#include "generic.hpp"
#include "Ship.hpp"
#include "Vector.hpp"

using namespace std;
	
class DirectionDrawer : public CEvent {
	private:
	FPSmanager* 	FPS_manager;	
	SDL_Surface*	Surf_Display;
	SDL_Surface* 	Surf_BG;
	SDL_Surface*	Surf_Ship1;
	SDL_Surface*	Surf_Ship2;
	int Running;

	private:
	Ship ship;
//	Ship ship2;

	public:
	DirectionDrawer();
	bool PrepSDL();
	void Init();
	void MainLoop();
	void Render();
	void OnEvent(SDL_Event*);
	void OnKeyDown(SDLKey, SDLMod, Uint16);
	void OnKeyUp(SDLKey, SDLMod, Uint16);
};

DirectionDrawer::DirectionDrawer() {
	Surf_Display	= NULL;
	Surf_Ship1	= NULL;
	Surf_Ship2	= NULL;
	Surf_BG		= NULL;
	Running		= 1;
}

void DirectionDrawer::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch(sym) { 
		case SDLK_ESCAPE: 
			Running = 0;
			break;

		case SDLK_RIGHT:
			ship.Rotate((-1 * ship.rotate_speed));
			break;

		case SDLK_LEFT:
			ship.Rotate(ship.rotate_speed);
			break;

		case SDLK_DOWN:
			ship.Accelerating(-1);
			break;

		case SDLK_UP:
			ship.Accelerating(1);
			
			break;
/*
// player 2 		
		case SDLK_d:
			ship2.Rotate((-1 * ship2.rotate_speed));
			break;

		case SDLK_a:
			ship2.Rotate(ship2.rotate_speed);
			break;
*/

	}
}

void DirectionDrawer::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch(sym) { 
		case SDLK_RIGHT:
			ship.Rotate(ship.rotate_speed);
			break;

		case SDLK_LEFT:
			ship.Rotate((-1 * ship.rotate_speed));
			break;

		case SDLK_DOWN:
			ship.Accelerating(1);
			break;

		case SDLK_UP:
			ship.Accelerating(-1);
			break;
/*
// player 2
		case SDLK_d:
			ship2.Rotate(ship2.rotate_speed);
			break;

		case SDLK_a:
			ship2.Rotate((-1 * ship2.rotate_speed));
			break;
*/
	}
}

void DirectionDrawer::Init() { 
	SDL_initFramerate( FPS_manager );
	SDL_setFramerate( FPS_manager, FRAME_RATE );

	ship.ShipCords.x = 500;
	ship.ShipCords.y = 500;
	ship.acc_speed = 1.5;
	ship.max_speed = 10;
	ship.rotate_speed = 10;
	ship.LoadSurface("./gfx/Ship1.png");
}

bool DirectionDrawer::PrepSDL() { 
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	if ((Surf_Display = SDL_SetVideoMode(BOARD_X,BOARD_Y,32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
		return false;

	Surf_BG =  CSurface::OnLoad("./gfx/bg.png");

	return true; 
}

void DirectionDrawer::OnEvent(SDL_Event* Event) {
	CEvent::OnEvent(Event);
}

void DirectionDrawer::MainLoop() {
	int i = 0;

	while (Running == 1) {

		SDL_Event event;

		while (SDL_PollEvent(&event))
			OnEvent(&event);

		ship.NextShip();
		//ship2.NextShip();

		//cout << ship.x << "\t" << ship.y << endl;
		Render();
		SDL_framerateDelay( FPS_manager );

	}
}

void DirectionDrawer::Render() {
	//Draw_Circle(Surf_Display,ship.x,ship.y,(rand() % 2), (rand()));
	//Draw_Pixel(Surf_Display,ship.x,ship.y,(rand()));
	//Draw_Line(Surf_Display,ship.last_x,ship.last_y,ship.x,ship.y,(rand()));
//	SDL_FillRect(Surf_Display,NULL,0);
//	CSurface::OnDraw(Surf_Display,Surf_Ship1,ship.x,ship.y,((int)ship.degree * 45),0,45,45);
	//CSurface::OnDraw(Surf_Display,Surf_Ship,ship.x,ship.y, 0, 0, 30, 30);

	//background
	CSurface::OnDraw(Surf_Display,Surf_BG, 0, 0);

	//first ship




	for (list<Cords>::const_iterator p = ship.Last_ShipCords.begin(); p != ship.Last_ShipCords.end(); p++) { 
//		SDL_Surface* tmp_surf = ship.GetSurface((vsurf_sz)p->degree);
//		SDL_SetAlpha(tmp_surf, SDL_SRCALPHA | SDL_RLEACCEL , 5);
//		SDL_Surface* xxx = SDL_DisplayFormat(tmp_surf); 
//		CSurface::OnDraw(Surf_Display,xxx, (p->x - (tmp_surf->w / 2)), (p->y - (tmp_surf->h /2 )));

		Draw_FillCircle(Surf_Display,p->x,p->y,4,55555);

	}

	SDL_Surface* tmp_surf = ship.GetSurface((vsurf_sz)ship.ShipCords.degree);
	CSurface::OnDraw(Surf_Display,tmp_surf, (ship.ShipCords.x - (tmp_surf->w / 2)), (ship.ShipCords.y - (tmp_surf->h /2 )));
/*
	SDL_Surface* tmp2_surf = ship2.GetSurface();
	CSurface::OnDraw(Surf_Display,tmp2_surf, (ship2.x - (tmp2_surf->w / 2)), (ship2.y - (tmp2_surf->h /2 )));
*/
	// sdl stuff
	SDL_Flip(Surf_Display);
}

int main(int argc, char* argv[]) {
	DirectionDrawer Board;

	if (!Board.PrepSDL())
		return 1;

	Board.Init();
	Board.MainLoop();


	return 0;
}
