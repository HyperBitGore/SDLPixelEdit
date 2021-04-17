#pragma once
#include <iostream>
#include <time.h>
#include "SDL.h"


namespace Gore {
	class Edit {
	private:
		Uint64 LAST = 0;
		Uint64 NOW = SDL_GetPerformanceCounter();
	public:
		//Utility functions
		double getDelta() {
			double delta = 0;
			LAST = NOW;
			NOW = SDL_GetPerformanceCounter();
			delta = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
			delta = delta * 0.001;
			return delta;
		}
		//Set the pixel color of a surface
		//If you wanna use textures, just convert the surface to a texture
		void setPixel(SDL_Surface* tex, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
			SDL_LockSurface(tex);
			Uint32* pixels = (Uint32*)tex->pixels;
			Uint32* pixel = pixels + (y*tex->w) + x;
			*pixel = SDL_MapRGB(tex->format, r, g, b);
			SDL_UnlockSurface(tex);
		}
		void copyPixel(SDL_Surface* surf, int x, int y, Uint32 pixel) {
			SDL_LockSurface(surf);
			Uint32 *pixels = (Uint32*)surf->pixels;
			pixels[(y*surf->w) + x] = pixel;
			SDL_UnlockSurface(surf);
		}
		//returns the pixel color of a pixel at a certain x and y
		Uint32 getPixel(SDL_Surface* surf, int x, int y) {
			SDL_LockSurface(surf);
			Uint32 *pixels = (Uint32 *)surf->pixels;
			SDL_UnlockSurface(surf);
			Uint32 pixel = pixels[(y * surf->w) + x];
			return pixel;
		}
		//returns rgb value of pixel
		Uint32 getPixelRGB(SDL_Surface* surf, int x, int y) {
			SDL_LockSurface(surf);
			Uint32 *pixels = (Uint32 *)surf->pixels;
			Uint32 pixel = pixels[(y * surf->w) + x];
			Uint8 g = 0;
			Uint8 b = 0;
			Uint8 r = 0;
			SDL_GetRGB(pixel, surf->format, &r, &g, &b);
			std::cout << (unsigned)r << std::endl;
			std::cout << (unsigned)g << std::endl;
			std::cout << (unsigned)b << std::endl;
			SDL_UnlockSurface(surf);
			return pixel;
		}
	};
}