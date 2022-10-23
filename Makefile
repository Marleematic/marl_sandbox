build:
	gcc ./main.c -lSDL2 -L ../marl_lib -lmarl -lGL -I ../marl_lib/include/ -o sandcastle
