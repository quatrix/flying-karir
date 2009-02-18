#include <iostream>
#include <cstdlib>
#include <math.h>
#include <SDL.h>
#include "CSurface.h"
#include "CEvent.h"
#include <SDL_rotozoom.h>
#include <vector>

#define PI 3.14159265

using namespace std;
typedef vector<SDL_Surface*>::size_type vsurf_sz;

class Direction {
	public:
	double x_factor;
	double y_factor;

	public:
	void CalcFactor(double);
};

void Direction::CalcFactor(double degree) {
	static const double arcus = 2. * PI / 360; 
	
	x_factor = cos( degree * arcus );
	y_factor = sin( degree * arcus );

	//cout << x_factor << "\t" << y_factor << endl;
}

// this is classic oo right there
class Point {

	public:
	double x;
	double y;
	double last_x;
	double last_y;
	double space_max_x;
	double space_max_y;

	private:
	double rotate;
	vector<SDL_Surface*> ship_surf;


	public:
	double degree;
	double speed;
	double rotate_speed;


	public:
	Point();
	void NextPoint();
	void Rotate(double);
	void Rotate();
	void LoadSurface(char*);
  SDL_Surface* GetSurface();
};

Point::Point() : degree(0), rotate(0),  rotate_speed(0) {}

void Point::Rotate(double i) { 
	rotate += i;
}

void Point::LoadSurface(char* filename) {

	SDL_Surface* origin =  CSurface::OnLoad(filename);

	for (vsurf_sz i = 0; i <= 360; i++) {
		SDL_Surface* tmp_surf = rotozoomSurface(origin, i, 2.0, 0);
		ship_surf.push_back(tmp_surf);	
	}
	
}

SDL_Surface* Point::GetSurface() {
	return ship_surf[(vsurf_sz)degree];
}

void Point::Rotate() { 
	degree += rotate;
	cout << degree << endl;
	
	if (degree < 0) 
		degree = 360 - rotate_speed;
	else if (degree > 360) 
		degree = rotate_speed;
	cout << degree << endl;
}

void Point::NextPoint() {
	Direction d;
	Rotate();

	cout << "rotate: " << rotate << endl;
	
	d.CalcFactor(degree);
	
	last_x = x;
	last_y = y;

	x -= speed * d.y_factor;
	y -= speed * d.x_factor;

	if (x > space_max_x) 
		x = 0;
	else if (x < 0) 
		x = space_max_x;

	if (y > space_max_y) 
		y = 0;
	else if (y < 0) 
		y = space_max_y;

}



class DirectionDrawer : public CEvent {
	private:
	SDL_Surface*	Surf_Display;
	SDL_Surface* 	Surf_BG;
	SDL_Surface*	Surf_Ship1;
	SDL_Surface*	Surf_Ship2;
	int Running;

	private:
	Point point;

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
	Surf_Display = NULL;
	Surf_Ship1 	 = NULL;
	Surf_Ship2 	 = NULL;
	Surf_BG    	 = NULL;
	Running = 1;
}

void DirectionDrawer::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch(sym) { 
		case SDLK_ESCAPE: 
			Running = 0;
			break;

		case SDLK_RIGHT:
			point.Rotate((-1 * point.rotate_speed));
			break;

		case SDLK_LEFT:
			point.Rotate(point.rotate_speed);
			break;

		case SDLK_DOWN:
			break;

		case SDLK_UP:
			break;

	}
}

void DirectionDrawer::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch(sym) { 
		case SDLK_RIGHT:
			point.Rotate(point.rotate_speed);
			break;

		case SDLK_LEFT:
			point.Rotate((-1 * point.rotate_speed));
			break;

		case SDLK_DOWN:
			break;

		case SDLK_UP:
			break;

	}
}

void DirectionDrawer::Init() { 
	point.x = 300;
	point.y = 300;
	point.speed = 0;
	point.rotate_speed = 1.5;
	point.space_max_x = 600;
	point.space_max_y = 600;
	point.LoadSurface("./gfx/Ship2.png");
}

bool DirectionDrawer::PrepSDL() { 
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	if ((Surf_Display = SDL_SetVideoMode(600,600,32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
		return false;


/*	if ((Surf_Ship1 = CSurface::OnLoad("./gfx/ship1_montage.png")) == NULL) {
		return false;
	}

	if ((Surf_Ship2 = CSurface::OnLoad("./gfx/ship2_montage.png")) == NULL) {
		return false;
	}
*/

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

		point.NextPoint();

		//cout << point.x << "\t" << point.y << endl;
		Render();
		//SDL_Delay(1);
	}
}

void DirectionDrawer::Render() {
	//Draw_Circle(Surf_Display,point.x,point.y,(rand() % 2), (rand()));
	//Draw_Pixel(Surf_Display,point.x,point.y,(rand()));
	//Draw_Line(Surf_Display,point.last_x,point.last_y,point.x,point.y,(rand()));
	SDL_FillRect(Surf_Display,NULL,0);
//	CSurface::OnDraw(Surf_Display,Surf_Ship1,point.x,point.y,((int)point.degree * 45),0,45,45);
	//CSurface::OnDraw(Surf_Display,Surf_Point,point.x,point.y, 0, 0, 30, 30);
	CSurface::OnDraw(Surf_Display,point.GetSurface(), point.x, point.y);
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
