#include <SDL2/SDL.h>
#include "marl.h"

int main(int argc, char **argv) {
	MARL_Loop *loop = MARL_CreateLoop(16);
	MARL_Window *window = MARL_CreateWindow("Hello World", 0, 0, 1600, 900);
	SDL_Init(SDL_INIT_EVENTS);
	short isRunning = 1;
	SDL_Event event;
	while(isRunning) {
		MARL_LoopWait(loop, &event);
//		MARL_LoopCatchUp(loop);
		if(1) {
			if(event.type == SDL_QUIT) {
				isRunning = 0;
			} else if(event.type == SDL_KEYDOWN) {
				SDL_Log("Found Key Event!!!");
				SDL_Delay(50);
		}
		}
	}
	MARL_DestroyLoop(loop);
	MARL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
