#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <math.h>

#include <SDL.h>
#include <SDL_rotozoom.h>
#include <SDL_framerate.h>
#include <SDL_gfxPrimitives.h>

#include "CSurface.h"
#include "CEvent.h"
#include "generic.hpp"
#include "Ship.hpp"
#include "Vector.hpp"

using namespace std;
	
class DirectionDrawer : public CEvent {
	private:
	SDL_Surface*	Surf_Display;
	SDL_Surface* 	Surf_BG;
	SDL_Surface*	Surf_Ship1;
	SDL_Surface*	Surf_Ship2;
	int Running;
	int Pause;
	int oioioi;

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
	void DirectionDrawer::DrawVec(Vector, Cords,Uint8);
};

DirectionDrawer::DirectionDrawer() {
	Surf_Display	= NULL;
	Surf_Ship1	= NULL;
	Surf_Ship2	= NULL;
	Surf_BG		= NULL;
	Running		= 1;
	Pause		= -1;
}

void DirectionDrawer::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch(sym) { 
		case SDLK_ESCAPE: 
			Running = 0;
			break;

		case SDLK_SPACE: 
			Pause *= -1;
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

		if (Pause < 0) { 
			ship.NextShip();

			Render();

			SDL_Delay(1000 / FRAME_RATE);
		}
		
		// reseting vectors	
		ship.ClearVectors();

	}
}

void DirectionDrawer::DrawVec(Vector v, Cords c,Uint8 color) {
	double x = c.x + ( 100 * v.force * v.X());
	double y = c.y - ( 100 * v.force * v.Y());
	lineRGBA(Surf_Display,c.x,c.y,x,y,color,0 ,0 ,255);
}

void DirectionDrawer::Render() {
	//background
	CSurface::OnDraw(Surf_Display,Surf_BG, 0, 0);


	if (DRAW_VECTORS == 1) {
		DrawVec(ship.ship_vec,ship.ShipCords,100);

		// draw ship_vec
		Uint8 color = 200;
		for (vec_iter i = ship.ship_vectors.begin(); i != ship.ship_vectors.end(); i++) {
			DrawVec(*i,ship.ShipCords,color);
			color += 30;
		}
	}
	
	if (DRAW_TRACE == 1) { 
		// movement traces 
		for (list<Cords>::const_iterator p = ship.Last_ShipCords.begin(); p != ship.Last_ShipCords.end(); p++) { 
			filledCircleRGBA(Surf_Display,p->x,p->y,4,255,255,0,255);
		}
	}

	// ship itself
	SDL_Surface* tmp_surf = ship.GetSurface((vsurf_sz)ship.ShipCords.degree);
	CSurface::OnDraw(Surf_Display,tmp_surf, (ship.ShipCords.x - (tmp_surf->w / 2)), (ship.ShipCords.y - (tmp_surf->h /2 )));


	
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
