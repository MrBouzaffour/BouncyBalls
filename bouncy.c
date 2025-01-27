#include <stdio.h>
#include <SDL2/SDL.h>

// Window dimensions
#define HEIGHT 600
#define WIDTH 900

#define COLOR_WHITE 0Xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_BACKGROUND 0X02020202
#define COLOR_GRAY 0xf1f1f1f1
#define COLOR_TRAJECTORY 0xff763b

#define A_GRAVITY 0.6
#define DAMPENING 0.9

#define TRAJECTORY_LENGTH 100

#define TRAJECTORY_WIDTH 10

struct Circle
{
	double x;
	double y;
	double radius;
	double v_x; // velocity
	double v_y;
};


void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color)
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
				SDL_FillRect(surface, &pixel, color);
			}
		}
	}
}


void FillTrajectory(SDL_Surface* surface, struct Circle trajectory[TRAJECTORY_LENGTH], int current_trajectory_index)
{
    for (int i = 0; i < current_trajectory_index; i++)
    {
        // Calculate size based on the reverse index position
        double trajectory_size = TRAJECTORY_WIDTH * (current_trajectory_index - i) / (double)current_trajectory_index;
        trajectory[i].radius = trajectory_size;
        FillCircle(surface, trajectory[i], COLOR_TRAJECTORY);
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
		circle->v_x = -circle->v_x * DAMPENING;
	}

	// Refect off the bottom and top edges
	if (circle->y + circle->radius > HEIGHT)
        {
                circle->y = HEIGHT - circle->radius;
                circle->v_y = -circle->v_y * DAMPENING;
        }
	
	if ( circle->y  - circle->radius < 0)
	{
		circle->y = circle->radius;
		circle->v_y = -circle->v_y * DAMPENING;
	}
	if ( circle->x  - circle->radius < 0)
        {
                circle->x = circle->radius;
                circle->v_x = -circle->v_x * DAMPENING;
        }
}

void UpdateTrajectory(struct Circle trajectory[TRAJECTORY_LENGTH], struct Circle circle, int* current_index)
{
    if (*current_index >= TRAJECTORY_LENGTH) {
        // Shift all elements one position to the left
        for (int i = 1; i < TRAJECTORY_LENGTH; i++) {
            trajectory[i - 1] = trajectory[i];
        }
        // Place the new circle at the last position
        trajectory[TRAJECTORY_LENGTH - 1] = circle;
    } else {
        // Simply add the new circle at the current index if not full
        trajectory[*current_index] = circle;
        (*current_index)++;
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
					       	WIDTH, HEIGHT, SDL_WINDOW_BORDERLESS);
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

        struct Circle circle = (struct Circle) {200, 200, 80, 50, 50};
	struct Circle trajectory[TRAJECTORY_LENGTH];	
	int current_trajectory_index  = 0;

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
		
		SDL_FillRect(surface, &erase_rect, COLOR_BACKGROUND);
		FillTrajectory(surface, trajectory, current_trajectory_index);
		FillCircle(surface, circle, COLOR_WHITE);

		step(&circle);
		UpdateTrajectory(trajectory, circle, &current_trajectory_index);
		
		// Update the trajectory
       		if (current_trajectory_index < TRAJECTORY_LENGTH){ 
         		++current_trajectory_index;
		}
		SDL_UpdateWindowSurface(window);
		
		SDL_Delay(20);
	}

	// Clean up
	SDL_DestroyWindow(window);
	SDL_Quit();
    	return 0;
}
