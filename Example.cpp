#include "PixelEdit.h"
#include <vector>
#undef main
//x and y are float for delta time use later
struct Particle {
	float x;
	float y;
	int type;
	Uint32 pixel;
	void(*update)(Particle *p);
};


//Declaring global variables
Gore::Edit engine;
SDL_Surface *surface;
SDL_Window *window;
std::vector<Particle> particles;
SDL_Renderer *rend;
bool exitf = false;
//Functions that particle update pointer points to
void sandUpdate(Particle *p) {
	engine.setPixelRGB(surface, (*p).x, (*p).y, 0, 0, 0);
	if (engine.getPixel(surface, (*p).x, (*p).y + 1) == 0 && (*p).y < 299) {
		(*p).y++;
	}
	else if (engine.getPixel(surface, (*p).x + 1, (*p).y + 1) == 0 && (*p).y < 299) {
		(*p).y++;
		(*p).x++;
	}
	else if (engine.getPixel(surface, (*p).x - 1, (*p).y + 1) == 0 && (*p).y < 299) {
		(*p).y++;
		(*p).x--;
	}
	engine.setPixel(surface, (*p).x, (*p).y, (*p).pixel);
}
void waterUpdate(Particle *p) {
	engine.setPixelRGB(surface, (*p).x, (*p).y, 0, 0, 0);
	if (engine.getPixel(surface, (*p).x, (*p).y + 1) == 0 && (*p).y < 299) {
		(*p).y++;
	}
	else if (engine.getPixel(surface, (*p).x + 1, (*p).y + 1) == 0 && (*p).y < 299) {
		(*p).y++;
		(*p).x++;
	}
	else if (engine.getPixel(surface, (*p).x - 1, (*p).y + 1) == 0 && (*p).y < 299) {
		(*p).y++;
		(*p).x--;
	}
	else if (engine.getPixel(surface, (*p).x + 1, (*p).y) == 0 && (*p).y < 299) {
		(*p).x++;
	}
	else if (engine.getPixel(surface, (*p).x - 1, (*p).y) == 0 && (*p).y < 299) {
		(*p).x--;
	}
	engine.setPixel(surface, (*p).x, (*p).y, (*p).pixel);
}
void woodUpdate(Particle *p) {
	engine.setPixelRGB(surface, (*p).x, (*p).y, 0, 0, 0);
	engine.setPixel(surface, (*p).x, (*p).y, (*p).pixel);
}


int main(int argc, char **argv) {
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_EVERYTHING) > 0) {
		std::cout << "SDL Init failed: " << SDL_GetError << std::endl;
	}
	//Init global and needed variables
	window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 300, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(window, -1, 0);
	surface = SDL_CreateRGBSurfaceWithFormat(0, 300, 300, 32, SDL_PIXELFORMAT_RGB888);
	//How to use RGB correctly
	//engine.setPixelRGB(surface, 1, 1, 255, 20, 20);
//	Gore::RGB c = engine.getPixelRGB(surface, 1, 1);
//	std::cout << (unsigned)c.r << std::endl;
	int mode = 1;
	SDL_Rect testm;
	testm.x = 0;
	testm.y = 0;
	testm.w = 300;
	testm.h = 300;
	//Yellow: 16776960 Blue: 33023 Brown: 4789268
	while (!exitf) {
		double delta = engine.getDelta();
		//std::cout << delta << std::endl;
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				exitf = true;
				break;
			case SDL_KEYUP:
				//Changing particle creation mode
				switch (e.key.keysym.sym) {
				case SDLK_r:
					if (mode < 3) {
						mode++;
					}
					else if (mode > 2) {
						mode = 1;
					}
					break;
				}
				break;
			}
		}
		//Set draw color black and clear the screen with it
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
		SDL_RenderClear(rend);
		//Main logic
		for (auto& i : particles) {
			i.update(&i);
		}
		//Input and creation of new particles
		int x;
		int y;
		//Gets when left mouse button is down and creates a new particle and pushes it back into particles array
		if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			Particle p;
			p.type = mode;
			switch (mode) {
			case 1:	
				p.x = x;
				p.y = y;
				p.pixel = 16776960;
				p.update = &sandUpdate;
				break;
			case 2:
				p.x = x;
				p.y = y;
				p.pixel = 33023;
				p.update = &waterUpdate;
				break;
			case 3:
				p.x = x;
				p.y = y;
				p.pixel = 4789268;
				p.update = &woodUpdate;
				break;
			}
			particles.push_back(p);
		}
		//Convert the screen surface to a texture and then draw it and destroy its data
		SDL_Texture* test = SDL_CreateTextureFromSurface(rend, surface);
		//Draw the screen texture
		SDL_RenderCopy(rend, test, NULL, &testm);
		//Need this line if you are using renderer else nothing will draw
		SDL_RenderPresent(rend);
		SDL_DestroyTexture(test);
	}

	return 0;
}