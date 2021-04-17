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

void sandUpdate(Particle *p) {
	engine.setPixel(surface, (*p).x, (*p).y, 0, 0, 0);
	if (engine.getPixel(surface, (*p).x, (*p).y + 1) == 0) {
		(*p).y++;
	}
	else if (engine.getPixel(surface, (*p).x + 1, (*p).y + 1) == 0) {
		(*p).y++;
		(*p).x++;
	}
	else if (engine.getPixel(surface, (*p).x - 1, (*p).y + 1) == 0) {
		(*p).y++;
		(*p).x--;
	}
	if ((*p).y > 299) {
		(*p).y = 299;
	}
	engine.copyPixel(surface, (*p).x, (*p).y, (*p).pixel);
}

int main(int argc, char **argv) {
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_EVERYTHING) > 0) {
		std::cout << "SDL Init failed: " << SDL_GetError << std::endl;
	}
	//Init global and needed variables
	window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 300, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(window, -1, 0);
	surface = SDL_GetWindowSurface(window);
	/*engine.setPixel(surface, 1, 1, 73, 20, 20);
	std::cout << engine.getPixel(surface, 1, 1) << std::endl;*/
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
			}
		}
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
		SDL_RenderClear(rend);
		//Main logic
		for (auto& i : particles) {
			i.update(&i);
		}
		//Input and creation of new particles
		int x;
		int y;
		if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			Particle p;
			p.x = x;
			p.y = y;
			p.pixel = 16776960;
			p.type = 1;
			p.update = &sandUpdate;
			particles.push_back(p);
		}
		//Convert the screen surface to a texture and then draw it and destroy its data
		SDL_Texture* test = SDL_CreateTextureFromSurface(rend, surface);
		SDL_RenderCopy(rend, test, NULL, &testm);
		SDL_RenderPresent(rend);
		SDL_DestroyTexture(test);
	}

	return 0;
}