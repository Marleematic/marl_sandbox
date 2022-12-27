#include <glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "marl.h"

#define PI 3.14159265358979323846

int main(int argc, char **argv) {
	
	SDL_Init(SDL_INIT_VIDEO);
	
	MARL_Loop *loop = MARL_CreateLoop(16);
	MARL_Window *window = MARL_CreateWindow("Hello World", 0, 0, 1600, 900);

	short isRunning = 1;
	SDL_Event event;
	long second = SDL_GetTicks64() + 1000;
	long ticks = 0;
	int loopAction = 0;

	int mouseX = 0;
	int mouseY = 0;
	int evtype = 0;
	while(isRunning) {
		loopAction = MARL_LoopWait(loop, &event);
		if(loopAction == 1) {
			if(event.type != evtype) {
				SDL_Log("-> %i\n", event.type);
				SDL_Log("---> %X\n", event.tfinger);
				SDL_Log("[[%b]]", event);
				evtype = event.type;
			}
			if(event.type == SDL_QUIT) {
				isRunning = 0;
			} else if(event.type == SDL_KEYDOWN) {
				SDL_Log("[%5i] Found Key Event!!!", loop->iteration);
			} else if(event.type == SDL_KEYUP) {
				SDL_Log("[%5i] Key went up!!!", loop->iteration);
			} else if(event.type == SDL_MOUSEMOTION) {
				mouseX = event.motion.x;
				mouseY = event.motion.y;
			} else if(event.type == SDL_FINGERDOWN) {
				SDL_Log("%i, %i, %f\n", event.tfinger.touchId, event.tfinger.fingerId, event.tfinger.pressure);
			}
		} else {

		}
	} 

	MARL_DestroyLoop(loop);
	MARL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
