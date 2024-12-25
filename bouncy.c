#include <stdio.h>
#include <SDL2/SDL.h>

// Window dimensions
#define HEIGHT 600
#define WIDTH 900

#define COLOR_WHITE 0Xffffffff
#define COLOR_BLACK 0x00000000

#define A_GRAVITY 0.2


struct Circle
{
	double x;
	double y;
	double radius;
	double v_x;
	double v_y;
};


void FillCircle(SDL_Surface* surface, struct Circle circle)
{
	double low_x = circle.x - circle.radius;
	double low_y = circle.y - circle.radius;
	double high_x = circle.x + circle.radius;
	double high_y = circle.y + circle.radius;

	double  radius_squared = circle.radius * circle.radius;

	for (double x = low_x; x < high_x; x++)
	{ 
		for (double y = low_y; y < high_y; y++ )
		{
			// we to check if it is int the circute
			int center_distance_squared = ( x - circle.x ) * ( x - circle.x) + ( y - circle.y) * ( y - circle.y );
			if ( center_distance_squared < radius_squared )
			{
				SDL_Rect pixel = (SDL_Rect) {x,y,1,1}; // pixel
				SDL_FillRect(surface, &pixel, COLOR_WHITE);
			}
		}
	}
}

void step(struct Circle* circle)
{
	// we calculate the new position
	

	// Update position based on velocity
	circle->x += circle->v_x;
	circle->y += circle->v_y;

	// Update vertical velocity based on gravity
	circle->v_y += A_GRAVITY;
	
	// Reflect off the right and left edges
	if (circle->x + circle->radius > WIDTH)
	{
		circle->x = WIDTH - circle->radius;
		circle->v_x = -circle->v_x;
	}

	// Refect off the bottom and top edges
	if (circle->y + circle->radius > HEIGHT)
        {
                circle->y = HEIGHT - circle->radius;
                circle->v_y = -circle->v_x;
        }
	
	if ( circle->y  - circle->radius < 0)
	{
		circle->y = circle->radius;
		circle->v_y = -circle->v_y;
	}
	if ( circle->x  - circle->radius < 0)
        {
                circle->x = circle->radius;
                circle->v_x = -circle->v_x;
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
	//SDL_Rect rect = (SDL_Rect) {200, 200, 200, 200};


        struct Circle circle = (struct Circle) {200, 200, 100, 0, 0};
	SDL_Rect erase_rect = (SDL_Rect){0, 0, WIDTH, HEIGHT};
	SDL_Event event;
	int simulation_running = 1;
	while (simulation_running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				simulation_running = 0;
			}

			// We close the window when we Space bar is pressed
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_SPACE)
				{
					simulation_running = 0;
				}
			}
		}
		SDL_FillRect(surface, &erase_rect, COLOR_BLACK);
		FillCircle(surface, circle);
		step(&circle);
		SDL_UpdateWindowSurface(window);
		
		SDL_Delay(20);
	
	}



	// Delay for a short while so we can see the window
	//SDL_Delay(3000);

	// Clean up
	SDL_DestroyWindow(window);
	SDL_Quit();
    	return 0;
}

