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

	private:
	vector<Ship> ships;

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

		case SDLK_p: 
			Pause *= -1;
			break;

		case SDLK_SPACE: {
			if (ships[0].CanFire()) 
				ships.push_back(ships[0].Fire());
			break;
		}

		case SDLK_RIGHT:
			ships[0].Rotate((-1 * ships[0].rotate_speed));
			break;

		case SDLK_LEFT:
			ships[0].Rotate(ships[0].rotate_speed);
			break;

		case SDLK_DOWN:
			ships[0].Accelerating(-1);
			break;

		case SDLK_UP:
			ships[0].Accelerating(1);
			
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
			ships[0].Rotate(ships[0].rotate_speed);
			break;

		case SDLK_LEFT:
			ships[0].Rotate((-1 * ships[0].rotate_speed));
			break;

		case SDLK_DOWN:
			ships[0].Accelerating(1);
			break;

		case SDLK_UP:
			ships[0].Accelerating(-1);
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
	Ship ship;
	ship.ShipCords.x = 500;
	ship.ShipCords.y = 500;
	ship.ShipCords.degree = 0;
	ship.acc_speed = 2;
	ship.max_speed = 10;
	ship.rotate_speed = 10;
	ship.fire_cost = 10;
	ship.hit_points = 100;
	ship.LoadSurface("./gfx/Ship1.png",ship.ship_surf);
	ship.LoadSurface("./gfx/Ship2.png",ship.missile_surf);

	ships.push_back(ship);
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
			for (ship_iter sp = ships.begin(); sp != ships.end(); sp++) 
				sp->NextShip();

			Render();

			SDL_Delay(1000 / FRAME_RATE);
		}
		
		// reseting vectors	
		for (ship_iter sp = ships.begin(); sp != ships.end(); sp++) {
			sp->ClearVectors();

			// deleting expired missiles
			if (sp->max_distance != 0 && sp->distance > sp->max_distance) {
				ships.erase(sp);
				sp--;
			}

		}





	}
}

void DirectionDrawer::DrawVec(Vector v, Cords c,Uint8 color) {
	double x = c.x + ( 100 * v.force * v.X());
	double y = c.y - ( 100 * v.force * v.Y());

	cout << "cx: " << c.x << " cy: " << c.y << " x: " << x << " y: " << y  << endl;
	lineRGBA(Surf_Display,c.x,c.y,x,y,0,color  / 2,color  ,255);
}

void DirectionDrawer::Render() {
	//background
	CSurface::OnDraw(Surf_Display,Surf_BG, 0, 0);


	if (DRAW_VECTORS == 1) {
		for (ship_iter sp = ships.begin(); sp != ships.end(); sp++) 
			DrawVec(sp->ship_vec,sp->ShipCords,100);

		// draw ship_vec
		Uint8 color = 200;
		for (ship_iter sp = ships.begin(); sp != ships.end(); sp++) {
			for (vec_iter i = sp->ship_vectors.begin(); i != sp->ship_vectors.end(); i++) {
				DrawVec(*i,sp->ShipCords,color);
				color += 30;
			}
		}
	}
	
	if (DRAW_TRACE == 1) { 
		// movement traces 
		for (ship_iter sp = ships.begin(); sp != ships.end(); sp++) 
			for (list<Cords>::const_iterator p = sp->Last_ShipCords.begin(); p != sp->Last_ShipCords.end(); p++) { 
				filledCircleRGBA(Surf_Display,p->x,p->y,4,255,255,0,255);
			}
	}

	// ship itself
	for (ship_iter sp = ships.begin(); sp != ships.end(); sp++) {
		SDL_Surface* tmp_surf = sp->GetSurface((vsurf_sz)sp->ShipCords.degree);
		CSurface::OnDraw(Surf_Display,tmp_surf, (sp->ShipCords.x - (tmp_surf->w / 2)), (sp->ShipCords.y - (tmp_surf->h /2 )));
	}

	
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
