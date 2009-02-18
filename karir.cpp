#include <iostream>
#include <cstdlib>
#include <math.h>
#include <SDL.h>
//#include <SDL_draw.h>
#include "CSurface.h"
#include "CEvent.h"

#define PI 3.14159265

using namespace std;

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


	public:
	double degree;
	double speed;

	public:
	Point();
	void NextPoint();
	void Rotate(double);
	void Rotate();
};

Point::Point() : degree(0), rotate(0) {}

void Point::Rotate(double i) { 
	rotate += i;
}

void Point::Rotate() { 
	degree += rotate;
	cout << degree << endl;
	
	if (degree < 0) 
		degree = 330;
	else if (degree > 360) 
		degree = 30;
	cout << degree << endl;
}

void Point::NextPoint() {
	Direction d;
	Rotate();

	cout << "rotate: " << rotate << endl;
	
	d.CalcFactor(degree);
	
	last_x = x;
	last_y = y;

	x += speed * d.y_factor;
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
	SDL_Surface*	Surf_Point;
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
	Surf_Point = NULL;
	Running = 1;
}

void DirectionDrawer::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch(sym) { 
		case SDLK_ESCAPE: 
			Running = 0;
			break;

		case SDLK_LEFT:
			point.Rotate(-1);
			break;

		case SDLK_RIGHT:
			point.Rotate(1);
			break;

		case SDLK_DOWN:
			break;

		case SDLK_UP:
			break;

	}
}

void DirectionDrawer::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch(sym) { 
		case SDLK_LEFT:
			point.Rotate(1);
			break;

		case SDLK_RIGHT:
			point.Rotate(-1);
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
	point.speed = 1;
	point.space_max_x = 600;
	point.space_max_y = 600;
}

bool DirectionDrawer::PrepSDL() { 
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	if ((Surf_Display = SDL_SetVideoMode(600,600,32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
		return false;

	if ((Surf_Point = CSurface::OnLoad("./karir.png")) == NULL) {
		return false;
	}

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
	CSurface::OnDraw(Surf_Display,Surf_Point,point.x,point.y,(point.degree * 30),0,30,30);
	//CSurface::OnDraw(Surf_Display,Surf_Point,point.x,point.y, 0, 0, 30, 30);
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
