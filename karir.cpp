#include <iostream>
#include <cstdlib>
#include <math.h>
#include <SDL.h>
#include "CSurface.h"
#include "CEvent.h"
#include <SDL_rotozoom.h>
#include <vector>

using namespace std;
typedef vector<SDL_Surface*>::size_type vsurf_sz;


double rad (double i) { 
	return i * M_PI / 180;
}

double deg (double i) { 
	return i * 180 / M_PI;
}

class Direction {
	public:
	double x_factor;
	double y_factor;

	public:
	void CalcFactor(double);
};

void Direction::CalcFactor(double degree) {
	x_factor = cos( rad(degree) );
	y_factor = sin( rad(degree) );

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
	double accelerating;
	vector<SDL_Surface*> ship_surf;


	public:
	double degree;
	double degree_vector;
	double speed;
	double rotate_speed;
	double max_speed;
	double acc_speed; 

	public:
	Point();
	void NextPoint();
	void Rotate(double);
	void Rotate();
	void LoadSurface(char*);
    SDL_Surface* GetSurface();
	void Accelerate(int);
	void Decelerate();
	void Accelerating(double);
};

Point::Point() : accelerating(0), acc_speed(0), max_speed(0), degree(0), degree_vector(0), rotate(0),  rotate_speed(0), speed(0) {}

void Point::Accelerating(double i) {
	accelerating += i;
}

void Point::Accelerate(int engine_degree = 0) { 


    cout << "degree-pre: " << degree_vector << endl;
	double orig_x = speed * cos(rad(degree_vector));
	double orig_y = speed * sin(rad(degree_vector));

    cout << "ox: " << orig_x << " oy: " << orig_y << endl;


	double new_x = acc_speed * cos(rad(degree - engine_degree));
//    cout << acc_speed << " * " << cos(rad(degree)) << " = " << new_x << endl;
	double new_y = acc_speed * sin(rad(degree - engine_degree));
//    cout << "new x: " << new_x << " new y: " << new_y << endl;

    cout << "nx: " << new_x << " ny: " << new_y << endl;

	double sum_x = orig_x + new_x;
	double sum_y = orig_y + new_y;

    cout << "sx: " << sum_x << " sy: " << sum_y << endl;

	double new_speed = sqrt(sum_x * sum_x + sum_y * sum_y);

    cout << "new speed: " << new_speed << endl;
	
	double new_degree = deg(atan(sum_y / sum_x));
	cout << "degree-mid: " << new_degree << " x: " << sum_x << " y: " << sum_y << endl;

	if (sum_x < 0 && sum_y >0) {
		new_degree += 180;
        cout << "x < 0 y > 0\n";
    }
	else if (sum_x < 0 && sum_y < 0) {
		new_degree += 180;
        cout << "x < 0 y < 0\n";
    }
	else if (sum_x > 0 && sum_y < 0) {
		new_degree += 360; 
        cout << "x > 0 y < 0\n";
    }


	cout << "degree-post: " << new_degree << " x: " << sum_x << " y: " << sum_y << endl;


	degree_vector = new_degree;
//    if (degree > 170 && degree < 190) 
//        degree_vector  = 180;

	if (new_speed <= max_speed) { 
		speed = new_speed;
        cout << "yes - speed: " << speed << endl;
    }

}

void Point::Decelerate() { 
    
//	if (speed > 0) 
//		speed = speed - acc_speed;
//    degree = degree_vector - 180;
    Accelerate(180);
	cout << "speed: " << speed << endl;
}

void Point::Rotate(double i) { 
	rotate += i;
}

void Point::LoadSurface(char* filename) {

	SDL_Surface* origin =  CSurface::OnLoad(filename);

	for (vsurf_sz i = 0; i <= 360; i++) {
		SDL_Surface* tmp_surf = rotozoomSurface(origin, i, 1.0, 0);
		ship_surf.push_back(tmp_surf);	
	}
	
}

SDL_Surface* Point::GetSurface() {
	return ship_surf[(vsurf_sz)degree];
}

void Point::Rotate() { 
	degree += rotate;
	//cout << degree << endl;
	
	if (degree < 0) 
		degree = 360 - rotate_speed;
	else if (degree > 360) 
		degree = rotate_speed;
	//cout << degree << endl;
}

void Point::NextPoint() {
	Direction d;
	Rotate();

	//cout << "accelerating: " << accelerating << endl;
	
	if (accelerating > 0) 
		Accelerate();
	else if (accelerating < 0)
		Decelerate();

	//cout << "rotate: " << rotate << endl;
	//cout << "degree_vector: " << degree_vector << endl;
	
	d.CalcFactor(degree_vector);
	
	last_x = x;
	last_y = y;

	x -= speed * d.y_factor;
	y -= speed * d.x_factor;
 //   cout << "speed: " << speed << endl;

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
	Point point2;

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
			point.Accelerating(-1);
			break;

		case SDLK_UP:
			point.Accelerating(1);
			
			break;

// player 2 		
		case SDLK_d:
			point2.Rotate((-1 * point2.rotate_speed));
			break;

		case SDLK_a:
			point2.Rotate(point2.rotate_speed);
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
			point.Accelerating(1);
			break;

		case SDLK_UP:
			point.Accelerating(-1);
			break;
// player 2
		case SDLK_d:
			point2.Rotate(point2.rotate_speed);
			break;

		case SDLK_a:
			point2.Rotate((-1 * point2.rotate_speed));
			break;
	}
}

void DirectionDrawer::Init() { 
	point.x = 500;
	point.y = 500;
	point.speed = 0;
	point.acc_speed = 5;
	point.max_speed = 20;
	point.rotate_speed = 7;
	point.space_max_x = 800;
	point.space_max_y = 600;
	point.LoadSurface("./gfx/Ship1.png");


	point2.x = 100;
	point2.y = 100;
	point2.speed = 0;
	point2.acc_speed = 10;
	point2.max_speed = 30;
	point2.rotate_speed = 10;
	point2.space_max_x = 800;
	point2.space_max_y = 600;
	point2.LoadSurface("./gfx/Ship2.png");

}

bool DirectionDrawer::PrepSDL() { 
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	if ((Surf_Display = SDL_SetVideoMode(800,600,32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
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

		point.NextPoint();
		point2.NextPoint();

		//cout << point.x << "\t" << point.y << endl;
		Render();
		SDL_Delay(50);
	}
}

void DirectionDrawer::Render() {
	//Draw_Circle(Surf_Display,point.x,point.y,(rand() % 2), (rand()));
	//Draw_Pixel(Surf_Display,point.x,point.y,(rand()));
	//Draw_Line(Surf_Display,point.last_x,point.last_y,point.x,point.y,(rand()));
	//SDL_FillRect(Surf_Display,NULL,0);
//	CSurface::OnDraw(Surf_Display,Surf_Ship1,point.x,point.y,((int)point.degree * 45),0,45,45);
	//CSurface::OnDraw(Surf_Display,Surf_Point,point.x,point.y, 0, 0, 30, 30);

	//background
	CSurface::OnDraw(Surf_Display,Surf_BG, 0, 0);

	//first ship
	SDL_Surface* tmp_surf = point.GetSurface();
	CSurface::OnDraw(Surf_Display,tmp_surf, (point.x - (tmp_surf->w / 2)), (point.y - (tmp_surf->h /2 )));

	SDL_Surface* tmp2_surf = point2.GetSurface();
	CSurface::OnDraw(Surf_Display,tmp2_surf, (point2.x - (tmp2_surf->w / 2)), (point2.y - (tmp2_surf->h /2 )));

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
