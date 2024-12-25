#include <stdio.h>
#include <SDL2/SDL.h>

// Window dimensions
#define HEIGHT 600
#define WIDTH 900

#define COLOR_WHITE 0Xffffffff
#define COLOR_BLACK 0x00000000

struct Circle
{
	int x;
	int y;
	int radius;
};


void FillCircle(SDL_Surface* surface, struct Circle circle)
{
	int low_x = circle.x - circle.radius;
	int low_y = circle.y - circle.radius;
	int high_x = circle.x + circle.radius;
	int high_y = circle.y + circle.radius;

	int  radius_squared = circle.radius * circle.radius;

	for (int x = low_x; x < high_x; x++)
	{    SDL_DestroyWindow(window);
		for (int y = low_y; y < high_y; y++ )
		{
			// we to check if it is int the circute
			int center_distance_squared = x*x + y*y;
			if ( center_distance_squared < radius_squared )
			{
				SDL_FillRect(surface,
						&((SDL_Rect){x,y,1,1}),
						COLOR_WHITE);
			}
		}
	}
}
int main() {
	// Initilized SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        	fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        	return 1;
    	}
	printf("SDL initialized successfully!\n");

    	// Crerate Window
    	SDL_Window* window = SDL_CreateWindow("Bouncy Ball",
						SDL_WINDOWPOS_CENTERED,
					       	SDL_WINDOWPOS_CENTERED,
					       	WIDTH, HEIGHT, 0);
	if (!window) {
        	fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        	SDL_Quit();
        	return 1;
    	}


	// Get the window surface
    	SDL_Surface* surface = SDL_GetWindowSurface(window);
	
	if (!surface) {
        	fprintf(stderr, "Failed to get window surface: %s\n", SDL_GetError());
        	SDL_DestroyWindow(window);
        	SDL_Quit();
       		return 1;
    	}

	// Define a rectangle
	SDL_Rect rect = (SDL_Rect) {200, 200, 200, 200};

	// Fill the rectangle with white
	SDL_FillRect(surface, &rect, 0xffffffff);
	
	// Update the window surface
	SDL_UpdateWindowSurface(window);
	
	// Delay for a short while so we can see the window
	SDL_Delay(3000);

	// Clean up
	SDL_DestroyWindow(window);
	SDL_Quit();
    	return 0;
}

